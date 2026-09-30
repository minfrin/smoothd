/**
 *    Copyright (C) 2020 Graham Leggett <minfrin@sharp.fm>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdio.h>
#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#include "config.h"

#define SOCKET_PATH "/run/smoothd.sock"
#define MAX_PACKET_SIZE 65536

static struct option long_options[] =
{
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

static int help(const char *name, const char *msg, int code)
{
    const char *n;

    n = strrchr(name, '/');
    if (!n) {
        n = name;
    }
    else {
        n++;
    }

    fprintf(code ? stderr : stdout,
            "%s\n"
            "\n"
            "NAME\n"
            "  %s - Send commands to the smooth daemon.\n"
            "\n"
            "SYNOPSIS\n"
            "  %s [-v] [-h] command ...\n"
            "\n"
            "DESCRIPTION\n"
            "\n"
            "  The smooth command sends tasks to the smooth daemon, whose job it is\n"
            "  to perform tasks like deleting or copying files that could overwhelm\n"
            "  a hard drive.\n"
            "\n"
            "OPTIONS\n"
            "  -h, --help  Display this help message.\n"
            "\n"
            "  -v, --version  Display the version number.\n"
            "\n"
            "RETURN VALUE\n"
            "  The smooth tool returns the return code from the\n"
            "  command being executed.\n"
            "\n"
            "  If the command could not be executed, or if the options\n"
            "  are invalid, the status 1 is returned.\n"
            "\n"
            "EXAMPLES\n"
            "  In this basic example, we delete some files.\n"
            "\n"
            "\t~$ smooth rm [pathname]\n"
            "\n"
            "AUTHOR\n"
            "  Graham Leggett <minfrin@sharp.fm>\n"
            "", msg ? msg : "", n, n);
    return code;
}

static int version()
{
    printf(PACKAGE_STRING "\n");
    return 0;
}

int main (int argc, char **argv)
{
    const char *name = argv[0];
    int c, status = 0, i, rv;

    char ff = 0xFF;

    while ((c = getopt_long(argc, argv, "hv", long_options, NULL)) != -1) {

        switch (c)
        {
        case 'h':
            return help(name, NULL, 0);

        case 'v':
            return version();

        default:
            return help(name, NULL, EXIT_FAILURE);

        }

    }

    if (optind == argc) {
        return help(name, "No command specified.\n", EXIT_FAILURE);
    }

    int fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (fd == -1) {
        if (errno == EPROTONOSUPPORT) {
            perror("SOCK_SEQPACKET is not supported on this platform");
        }
        else {
            perror("Socket creation failed");
        }
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    rv = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rv == -1) {
        perror(SOCKET_PATH);
        close(fd);
        return 1;
    }

    while (optind < argc) {

        rv = send(fd, argv[optind], strlen(argv[optind]) + 1, MSG_NOSIGNAL);
        if (rv == -1) {
            perror(SOCKET_PATH);
            close(fd);
            return 1;
        }

    }

    /* send FF to tell the other side we are done */
    rv = send(fd, &ff, sizeof(ff), MSG_NOSIGNAL);
    if (rv == -1) {
        perror(SOCKET_PATH);
        close(fd);
        return 1;
    }

    /* make space for one packet */
    char *packet_buffer;
    int max_packet_size = 0;
    socklen_t optlen = sizeof(max_packet_size);

    if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &max_packet_size, &optlen)) {
        perror(SOCKET_PATH);
        close(fd);
        return 1;
    }

    packet_buffer = malloc(max_packet_size);

    /* fetch the result if any */

    while (1) {

        ssize_t n = recv(fd, packet_buffer, max_packet_size, 0);

        /* we received a message */
        if (n > 0) {

            /* graceful termination? */
            if (n == 1 && packet_buffer[0] == ff) {
                free(packet_buffer);
                close(fd);
                return 0;
            }

            /* we have news from the server */
            else {
                fwrite(packet_buffer, 1, n, stderr);
            }
            
        }

        /* if we receive an empty packet, the server crashed */
        else if (n == 0) {
            fprintf(stderr, SOCKET_PATH ": server went away");
            free(packet_buffer);
            close(fd);
            return 2;
        }

        /* if we received an error, handle the error */
        else {
            if (errno == EINTR) {
                continue;
            }
            else {} {
                perror(SOCKET_PATH);
                free(packet_buffer);
                close(fd);
                return 1;
            }

        }
        
    }

    return status;
}
