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

#include "log.h"
#include "parse.h"

#define HTMC_CLI_HELP      "-h"
#define HTMC_CLI_LICENSE   "-l"
#define HTMC_CLI_TRANSLATE "-t"
#define HTMC_CLI_RUN       "-r"

const char *TMC_DISPLAY_HELP =
    "Usage: htmc [<option>] [<filename>]\n"
    "\n"
    "One of the following options must be specified:\n"
    "\t-h                               Shows this message\n"
    "\t-l                               Shows the MIT license\n"
    "\t-t <input file> <output file>    Transaltes an htmc file\n"
    "\t-r <input file>                  Compiles and runs an htmc file\n";

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

int cli_transalte(int argc, char *argv[]) {
  if (4 > argc) {
    log_fatal("insufficient number of arguments, epxcted: htmc -t <input file> "
              "<output file>");
    return EXIT_FAILURE;
  }

  const char *src_file_path = argv[2];
  const char *dst_file_path = argv[3];

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
  if (0 == r) {
    log_info("done");
    return r;
  }

  log_fatal("unable to complete correctly");
  return r;
}

int cli_run(int argc, char *argv[]) {
  log_fatal("operation not supported yet");
  return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
  if (2 > argc) {
    log_fatal("insufficient number of arguments, use -h option for help");
    return EXIT_FAILURE;
  }

  printf("htmc (dev version)\n");
  printf("Copyright (c) Alessandro Salerno 2024\n");
  printf("This software is under MIT license. Use -l option for more "
         "information.\n\n");

  const char *argument = argv[1];
  if (0 == strcmp(HTMC_CLI_HELP, argument)) {
    printf("%s", HTMC_DISPLAY_HELP);
  } else if (0 == strcmp(HTMC_CLI_LICENSE, argument)) {
    printf("%s", HTMC_DISPLAY_LICENSE);
  } else if (0 == strcmp(HTMC_CLI_TRANSLATE, argument)) {
    return cli_transalte(argc, argv);
  } else if (0 == strcmp(HTMC_CLI_RUN, argument)) {
    return cli_run(argc, argv);
  } else {
    log_fatal("unrecognized option");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
