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

// Temporary fix to avoid issues
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "util.h"

#define FMT_STR_LEN 2
#define OBJ_EXT_LEN 2
#define SO_EXT_LEN  3

#define STR_FMT_INSTANCES 4
#define OBJ_EXT_INSTANCES 2
#define SO_EXT_INSTANCES  1

const char *BASE_SO_CMD_FMT =
    "$(cc) -O2 -fPIC -Iinclude/ -o %s.o -c %s "
    "&& $(ld) -shared -o %s.so -L./bin --exclude-libs ALL --start-group "
    "-l:libhtmc.a %s.o --end-group";

int compile_c_output(const char *src_path, const char *dst_path) {
  int          ret_val      = EXIT_SUCCESS;
  const size_t src_path_len = strlen(src_path);
  const size_t cmd_len      = strlen(BASE_SO_CMD_FMT) -
                         (FMT_STR_LEN * STR_FMT_INSTANCES) +
                         (src_path_len * STR_FMT_INSTANCES);

#ifdef HTMC_CGI_INTF
  char *cmd = (char *)malloc(cmd_len);

  if (!cmd) {
    log_fatal("ran out of memory");
    ret_val = EXIT_FAILURE;
    goto cleanup;
  }

  sprintf(cmd, BASE_SO_CMD_FMT, src_path, src_path, src_path, src_path);
  ret_val = system(cmd);

cleanup:
  safe_free(cmd);
#endif

  return ret_val;
}
