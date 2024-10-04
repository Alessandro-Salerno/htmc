// MIT License
//
// Copyright (c) 2024 Alessandro Salerno
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libhtmc/libhtmc-internals.h"
#include "libhtmc/libhtmc.h"
#include "load.h"
#include "log.h"
#include "parse.h"

// This macro is used to avoid too many if
// statements in the main function
// Propose a better way if you have one
#define SET_FCN(f)                                                  \
  if (fcn_cli) {                                                    \
    log_fatal("more than one mutually exclusive option specified"); \
    return EXIT_FAILURE;                                            \
  }                                                                 \
  fcn_cli = f;

#define HTMC_FLAG_NO_SPLASH "-ns"
#define HTMC_FLAG_OUTPUT    "-o"

#define HTMC_FLAG_FULL_NO_SPLASH "--no-splash"
#define HTMC_FLAG_FULL_OUTPUT    "--output-path"

#define HTMC_CLI_HELP      "-h"
#define HTMC_CLI_LICENSE   "-l"
#define HTMC_CLI_VERSION   "-v"
#define HTMC_CLI_TRANSLATE "-t"
#define HTMC_CLI_LOAD_SO   "-s"
#define HTMC_CLI_RUN       "-r"

#define HTMC_CLI_FULL_HELP      "--help"
#define HTMC_CLI_FULL_LICENSE   "--license"
#define HTMC_CLI_FULL_VERSION   "--version"
#define HTMC_CLI_FULL_TRANSLATE "--translate"
#define HTMC_CLI_FULL_LOAD_SO   "--load-shared"
#define HTMC_CLI_FULL_RUN       "--run"

const char *HTMC_DISPLAY_HELP =
    "Usage: htmc [<flag(s)>] [<option>] [<argument(s)>]\n"
    "\n"
    "Optional flags:\n"
    "\t-ns, --no-splash                     Disables the program info splash "
    "text\n"
    "\t-o, --output-path {<file>|<path>}    Sets the output file or directory\n"
    "\n"
    "Mutually exclusive options:\n"
    "\t-h, --help           Shows this message\n"
    "\t-l, --license        Shows the MIT license\n"
    "\t-v, --version        Shows the htmc version string\n"
    "\t-t, --translate      Transaltes an htmc file\n"
    "\t-s, --load-shared    Loads and runs a shared object built with htmc\n"
    "\t-r, --run            Translate, compile, and runs an htmc file\n"
    "\n"
    "Example: translate `test.htmc` to `pagegen.c` without printing the splash "
    "text\n"
    "\t$ htmc -ns -t test.htmc -o pagegen.c\n"
    "\n"
    "If no option is specified, the program will launch in CGI mode.\n"
    "This allows other programs to call htmc for on-demande execution.\n";

const char *HTMC_DISPLAY_LICENSE =
    "MIT License"
    "\n"
    "Copyright (c) 2024 Alessandro Salerno\n"
    "\n"
    "Permission is hereby granted, free of charge, to any person obtaining "
    "a "
    "copy\n"
    "of this software and associated documentation files (the Software"
    "), to deal\n"
    "in the Software without restriction, including without limitation the "
    "rights\n"
    "to use, copy, modify, merge, publish, distribute, sublicense, and/or "
    "sell\n"
    "copies of the Software, and to permit persons to whom the Software "
    "is\n"
    "furnished to do so, subject to the following conditions:\n"
    "\n"
    "The above copyright notice and this permission notice shall be "
    "included "
    "in all\n"
    "copies or substantial portions of the Software.\n"
    "\n"
    "THE SOFTWARE IS PROVIDED AS IS "
    ", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
    "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF "
    "MERCHANTABILITY,\n"
    "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT "
    "SHALL "
    "THE\n"
    "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR "
    "OTHER\n"
    "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, "
    "ARISING "
    "FROM,\n"
    "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER "
    "DEALINGS IN "
    "THE\nSOFTWARE.\n";

// CLI Flags and options
// These are global so they're easier to access
const char *cliInputFile             = NULL;
const char *cliOutputFileOrDirectory = NULL;
bool        cliStopSplashText        = false;

void print_program_version() {
  printf("htmc version %s\n", EXT_HTMC_BUILD);
}

void print_program_info() {
  print_program_version();
  printf("Copyright (c) 2024 Alessandro Salerno\n");
  printf("This software is under MIT license. Use -l option for more "
         "information.\n\n");
}

int cli_help() {
  printf("%s", HTMC_DISPLAY_HELP);
  return EXIT_SUCCESS;
}

int cli_license() {
  printf("%s", HTMC_DISPLAY_LICENSE);
  return EXIT_SUCCESS;
}

int cli_version() {
  print_program_version();
  return EXIT_SUCCESS;
}

int cli_translate() {
  if (!cliInputFile) {
    log_fatal("input file required but not provided");
    return EXIT_FAILURE;
  }

  const char *src_file_path = cliInputFile;
  const char *dst_file_path = cliOutputFileOrDirectory;

  FILE *src_file = fopen(src_file_path, "r");
  if (!src_file) {
    log_fatal("invalid input file, no such file");
    return EXIT_FAILURE;
  }

  FILE *dst_file = fopen(dst_file_path, "w");
  if (!src_file) {
    log_fatal("invalid output file");
    return EXIT_FAILURE;
  }

  int r = parse_and_emit(src_file, dst_file);
  if (EXIT_SUCCESS == r) {
    log_info("done");
    return r;
  }

  log_fatal("unable to complete correctly");
  return r;
}

int cli_run() {
  log_fatal("operation not supported yet");
  return EXIT_FAILURE;
}

int cli_cgi(const char *query_string) {
  if (!query_string) {
    log_fatal("query not specified");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int cli_load_shared() {
  if (!cliInputFile) {
    log_fatal("input file required but not provided");
    return EXIT_FAILURE;
  }

  const char *so_file_path = cliInputFile;
  const char *query_string = getenv("QUERY_STRING");
  if (!query_string) {
    query_string = "";
  }

  htmc_handover_t handover = {.variant_id          = HTMC_BASE_HANDOVER,
                              .request_method      = "GET",
                              .query_string        = query_string,
                              .query_has_params    = false,
                              .query_param_sep_off = 0,
                              .vprintf             = impl_debug_vprintf,
                              .query_vscanf        = impl_debug_query_vscanf,
                              .form_vscanf         = impl_debug_form_vscanf,
                              .alloc               = impl_debug_alloc,
                              .free                = impl_debug_free,
                              .cleanup             = impl_debug_cleanup};

  return run_htmc_so(so_file_path, &handover);
}

int main(int argc, char *argv[]) {
  if (2 > argc) {
    return cli_cgi(getenv("QUERY_STRING"));
  }

  int (*fcn_cli)() = NULL;

  for (int i = 1; i < argc; i++) {
    const char *argument = argv[i];

    // -ns, no-splash
    if (0 == strcmp(HTMC_FLAG_NO_SPLASH, argument) ||
        0 == strcmp(HTMC_FLAG_FULL_NO_SPLASH, argument)) {
      cliStopSplashText = true;
    }

    // -o, --output-path
    else if (0 == strcmp(HTMC_FLAG_OUTPUT, argument) ||
             0 == strcmp(HTMC_FLAG_FULL_OUTPUT, argument)) {
      if (cliOutputFileOrDirectory) {
        log_fatal("multiple output flags are not supported");
        return EXIT_FAILURE;
      }

      // when -o flag is last argument
      if (argc - 1 == i) {
        log_fatal("expected value after output flag");
        return EXIT_FAILURE;
      }

      cliOutputFileOrDirectory = argv[++i];
      continue;
    }

    // -h, --help
    else if (0 == strcmp(HTMC_CLI_HELP, argument) ||
             0 == strcmp(HTMC_CLI_FULL_HELP, argument)) {
      SET_FCN(cli_help);
    }

    // -l, --license
    else if (0 == strcmp(HTMC_CLI_LICENSE, argument) ||
             0 == strcmp(HTMC_CLI_FULL_LICENSE, argument)) {
      SET_FCN(cli_license);
    }

    // -v, --version
    else if (0 == strcmp(HTMC_CLI_VERSION, argument) ||
             0 == strcmp(HTMC_CLI_FULL_VERSION, argument)) {
      cliStopSplashText = true;
      SET_FCN(cli_version);
    }

    // -t, --translate
    else if (0 == strcmp(HTMC_CLI_TRANSLATE, argument) ||
             0 == strcmp(HTMC_CLI_FULL_TRANSLATE, argument)) {
      SET_FCN(cli_translate);
    }

    // -s, --load-shared
    else if (0 == strcmp(HTMC_CLI_LOAD_SO, argument) ||
             0 == strcmp(HTMC_CLI_FULL_VERSION, argument)) {
      SET_FCN(cli_load_shared);
    }

    // -r, --run
    else if (0 == strcmp(HTMC_CLI_RUN, argument) ||
             0 == strcmp(HTMC_CLI_FULL_RUN, argument)) {
      SET_FCN(cli_run);
    }

    // No flag or option
    else if (!cliInputFile) {
      cliInputFile = argument;
    }

    else {
      print_program_info();
      log_fatal("too many input files");
      return EXIT_FAILURE;
    }
  }

  if (!cliStopSplashText) {
    print_program_info();
  }

  if (fcn_cli) {
    return fcn_cli();
  }

  return EXIT_FAILURE;
}
