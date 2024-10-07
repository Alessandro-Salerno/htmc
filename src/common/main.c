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

#include "cli.h"
#include "log.h"

#define HTMC_FLAG_NO_SPLASH "-ns"
#define HTMC_FLAG_OUTPUT    "-o"
#define HTMC_FLAG_LOG_LVL   "-ll"

#define HTMC_FLAG_FULL_NO_SPLASH "--no-splash"
#define HTMC_FLAG_FULL_OUTPUT    "--output-path"
#define HTMC_FLAG_FULL_LOG_LVL   "--log-level"

#define HTMC_CLI_HELP      "-h"
#define HTMC_CLI_LICENSE   "-l"
#define HTMC_CLI_VERSION   "-v"
#define HTMC_CLI_TRANSLATE "-t"
#define HTMC_CLI_COMPILE   "-c"
#define HTMC_CLI_BUILD     "-b"
#define HTMC_CLI_LOAD_SO   "-s"
#define HTMC_CLI_RUN       "-r"

#define HTMC_CLI_FULL_HELP      "--help"
#define HTMC_CLI_FULL_LICENSE   "--license"
#define HTMC_CLI_FULL_VERSION   "--version"
#define HTMC_CLI_FULL_TRANSLATE "--translate"
#define HTMC_CLI_FULL_COMPILE   "--compile"
#define HTMC_CLI_FULL_BUILD     "--build"
#define HTMC_CLI_FULL_LOAD_SO   "--load-shared"
#define HTMC_CLI_FULL_RUN       "--run"

#define HTMC_VPTR_FALSE (void *)0
#define HTMC_VPTR_TRUE  (void *)1

// CLI Flags and options
// These are global so they're easier to access
const char *cliInputFile             = NULL;
const char *cliOutputFileOrDirectory = NULL;
bool        cliStopSplashText        = false;
bool        logLevelSet              = false;

cli_opt_desc_t matches[] = {
    // Mutually exclusive options
    {HTMC_CLI_HELP, HTMC_CLI_FULL_HELP, NULL, NULL, false},
    {HTMC_CLI_LICENSE, HTMC_CLI_FULL_LICENSE, NULL, NULL, false},
    {HTMC_CLI_VERSION, HTMC_CLI_FULL_VERSION, NULL, NULL, false},
    {HTMC_CLI_TRANSLATE, HTMC_CLI_FULL_TRANSLATE, NULL, NULL, false},
    {HTMC_CLI_COMPILE, HTMC_CLI_FULL_COMPILE, NULL, NULL, false},
    {HTMC_CLI_BUILD, HTMC_CLI_FULL_BUILD, NULL, NULL, false},
    {HTMC_CLI_LOAD_SO, HTMC_CLI_FULL_LOAD_SO, NULL, NULL, false},
    {HTMC_CLI_RUN, HTMC_CLI_FULL_RUN, NULL, NULL, false},

    // Optional flags
    {HTMC_FLAG_NO_SPLASH,
     HTMC_FLAG_FULL_NO_SPLASH,
     flag_no_splash,
     &cliStopSplashText,
     false},

    {HTMC_FLAG_OUTPUT,
     HTMC_FLAG_FULL_OUTPUT,
     flag_output,
     &cliOutputFileOrDirectory,
     true},

    {HTMC_FLAG_LOG_LVL,
     HTMC_FLAG_FULL_LOG_LVL,
     flag_log_level,
     &logLevelSet,
     true},
};

cli_exec_t exec_matches[] = {
    cli_help,
    cli_license,
    cli_version,
    cli_translate,
    NULL, // cli_compile
    NULL, // cli_build
    cli_load_shared,
    cli_run,
};

int cli_cgi(const char *query_string) {
  if (!query_string) {
    log_fatal("query not specified");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  if (2 > argc) {
    return cli_cgi(getenv("QUERY_STRING"));
  }

  cli_exec_t fcn_cli = NULL;

  for (int i = 1; i < argc; i++) {
    bool        found_matching_option = false;
    const char *argument              = argv[i];
    const char *next                  = NULL;
    if (argc - i != i) {
      next = argv[i + 1];
    }

    for (int j = 0;
         !found_matching_option && j < sizeof matches / sizeof(cli_opt_desc_t);
         j++) {
      cli_opt_desc_t opt_desc = matches[j];

      if (0 != strcmp(opt_desc.short_option, argument) &&
          0 != strcmp(opt_desc.full_option, argument)) {
        continue;
      }

      found_matching_option = true;
      if (opt_desc.has_argument) {
        i++; // Advance outer loop to avoid issues
      }

      // If the handler is for mutually exclusive option
      if (NULL == opt_desc.handler && NULL == opt_desc.target_variable) {
        if (NULL != fcn_cli) {
          log_fatal("more than one mutually exclusive option specified");
          return EXIT_FAILURE;
        }

        fcn_cli = exec_matches[j];
        break;
      }

      // If the handler isfor a non-mutually exclusive option
      int fcn_ecode = opt_desc.handler(opt_desc.target_variable, next);

      if (EXIT_SUCCESS != fcn_ecode) {
        return fcn_ecode;
      }
    }

    // If no mathcing option was found
    // This argument is treated as the input file
    if (!found_matching_option) {
      if (NULL != cliInputFile) {
        log_fatal("too many input files");
        return EXIT_FAILURE;
      }

      cliInputFile = argument;
    }

    /*
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

    // -ll, --log-level
    else if (0 == strcmp(HTMC_FLAG_LOG_LVL, argument) ||
             0 == strcmp(HTMC_FLAG_FULL_LOG_LVL, argument)) {
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
      log_fatal("too many input files");
      return EXIT_FAILURE;
    }
    */
  }

  log_set_safe();

  if (!cliStopSplashText) {
    print_program_info();
  }

  if (fcn_cli) {
    return fcn_cli(cliInputFile, cliOutputFileOrDirectory);
  }

  return cli_cgi(getenv("QUERY_STRING"));
}