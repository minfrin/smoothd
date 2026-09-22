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

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

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
    int c, status = 0, i;

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

    return status;
}
