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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "libhtmc/libhtmc-internals.h"
#include "libhtmc/libhtmc.h"
#include "load.h"
#include "log.h"
#include "parse.h"

const char *HTMC_DISPLAY_HELP =
    "Usage: htmc [<flag(s)>] [<option>] [<argument(s)>]\n"
    "\n"
    "Optional flags:\n"
    "\t-ns, --no-splash                                  Disable the program "
    "info splash text\n"
    "\t-o,  --output-path {<file>|<path>}                Set the output file "
    "or directory\n"
    "\t-ll, --log-level  {all|info|warning|error|off}    Set the log level\n"
    "\n"
    "Mutually exclusive options:\n"
    "\t-h, --help           Display this message\n"
    "\t-l, --license        Display the MIT license\n"
    "\t-v, --version        Display the htmc version string\n"
    "\t-t, --translate      Transalte htmc source file into C source file\n"
    "\t-c, --compile        Compile a C source file to hmtc shared object\n"
    "\t-b, --build          Build shared object from htmc source file\n"
    "\t-s, --load-shared    Load and run an htmc shared object\n"
    "\t-r, --run            Run an htmc source file\n"
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

// Section
// Support functions

void print_program_version() {
  printf("htmc version %s\n", EXT_HTMC_BUILD);
}

void print_program_info() {
  print_program_version();
  printf("Copyright (c) 2024 Alessandro Salerno\n");
  printf("This software is under MIT license. Use -l option for more "
         "information.\n\n");
}

// Section
// Handler functions
// These are for optional flags

int flag_no_splash(void *target, const char *next) {
  *(bool *)target = true;
  return EXIT_SUCCESS;
}

int flag_output(void *target, const char *next) {
  const char **char_target = target;

  if (NULL != *char_target) {
    log_fatal("multiple output flags are not supported");
    return EXIT_FAILURE;
  }

  // when -o flag is last argument
  if (NULL == next) {
    log_fatal("expected value after output flag");
    return EXIT_FAILURE;
  }

  *char_target = next;
  return EXIT_SUCCESS;
}

int flag_log_level(void *target, const char *next) {
  bool *bool_target = target;

  if (*bool_target) {
    log_fatal("multiple log level flags are not supported");
    return EXIT_FAILURE;
  }

  if (NULL == next) {
    log_fatal("expected value after log level flag");
    return EXIT_FAILURE;
  }

  int log_level = log_translate_level(next);
  if (-1 == log_level) {
    log_fatal("unrecognized log level");
    return EXIT_FAILURE;
  }

  *bool_target = true;
  log_set_level(log_level);
  return EXIT_SUCCESS;
}

// Section
// Executable functions
// These are for mutually exclusive options

int cli_help(const char *input_file, const char *output_file) {
  printf("%s", HTMC_DISPLAY_HELP);
  return EXIT_SUCCESS;
}

int cli_license(const char *input_file, const char *output_file) {
  printf("%s", HTMC_DISPLAY_LICENSE);
  return EXIT_SUCCESS;
}

int cli_version(const char *input_file, const char *output_file) {
  print_program_version();
  return EXIT_SUCCESS;
}

int cli_translate(const char *input_file, const char *output_file) {
  if (!input_file) {
    log_fatal("input file required but not provided");
    return EXIT_FAILURE;
  }

  const char *src_file_path = input_file;
  const char *dst_file_path = output_file;

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

int cli_load_shared(const char *input_file, const char *output_file) {
  if (!input_file) {
    log_fatal("input file required but not provided");
    return EXIT_FAILURE;
  }

  const char *so_file_path = input_file;
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

int cli_run(const char *input_file, const char *output_file) {
  log_fatal("operation not supported yet");
  return EXIT_FAILURE;
}
