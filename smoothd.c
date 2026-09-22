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
            "  %s - Smooth daemon.\n"
            "\n"
            "SYNOPSIS\n"
            "  %s [-v] [-h] ...\n"
            "\n"
            "DESCRIPTION\n"
            "\n"
            "  The smooth daemon performs tasks that if done in parallel are likely\n"
            "  to overwhelm a hard drive or cause it to thrash.\n"
            "\n"
            "OPTIONS\n"
            "  -h, --help  Display this help message.\n"
            "\n"
            "  -v, --version  Display the version number.\n"
            "\n"
            "RETURN VALUE\n"
            "  The smooth daemon returns 0 when shut down gracefully.\n"
            "\n"
            "  If the unix domain socket could not be opened, or if the options\n"
            "  are invalid, the status 1 is returned.\n"
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
