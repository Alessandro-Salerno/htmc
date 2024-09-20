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

void run_c_file(const char *c_file_path) {
  const char *command     = "gcc -shared -fPIC -Iinclude/ src/libhtmc.c ";
  int         so_path_len = strlen(c_file_path) + 4;
  char       *so_path     = calloc(so_path_len, sizeof(char));
  sprintf(so_path, "%s%s", c_file_path, ".so");

  char *full_command = NULL;
  void *handle       = dlopen(so_path, RTLD_LAZY);
  if (handle) {
    goto run;
  }

  full_command = calloc(
      strlen(command) + so_path_len + strlen(c_file_path) + 10, sizeof(char));
  sprintf(full_command, "%s %s -o %s", command, c_file_path, so_path);

  (void)system(full_command);

  handle = dlopen(so_path, RTLD_LAZY);
  if (!handle) {
    printf("Error 1\n");
    goto cleanup;
  }

run:
  int (*htmc_main)(htmc_handover_t *) = dlsym(handle, "htmc_main");
  if (!htmc_main) {
    printf("Error 2\n");
    goto cleanup;
  }

  htmc_handover_t handover =
      (htmc_handover_t){.variant_id     = HTMC_BASE_HANDOVER,
                        .request_method = "GET",
                        .query_string   = "examples/test.htmc?param=ciao",
                        .vprintf        = impl_debug_vprintf,
                        .query_vscanf   = impl_debug_query_vscanf,
                        .form_vscanf    = impl_debug_form_vscanf,
                        .alloc          = impl_debug_alloc,
                        .free           = impl_debug_free,
                        .cleanup        = impl_debug_cleanup};

  htmc_main(&handover);

cleanup:
  if (full_command) {
    free(full_command);
  }
  free(so_path);
}

void parse_and_run(FILE *f, const char *file_path, int line) {
  int   path_len      = strlen(file_path);
  char *tmp_file_path = calloc(path_len + 16, sizeof(char));
  sprintf(tmp_file_path, "./tmp/%s.%d.c", file_path, line);
  for (char *cp = tmp_file_path + 7, ch; (ch = *cp); cp++) {
    if ('/' == ch) {
      *cp = '_';
      break;
    }
  }

  // FILE *c_file = fopen(tmp_file_path, "r");
  // if (c_file) {
  //   run_c_file(tmp_file_path);
  //   goto cleanup;
  // }

  FILE *c_file = fopen(tmp_file_path, "w");
  fprintf(c_file,
          "#include <stdio.h>\n#include\"libhtmc/libhtmc.h\"\nint "
          "htmc_main(htmc_handover_t *h) {htmc_bind(h);\n");

  char c;
  char last = 0;
  while (0 != fread(&c, 1, 1, f)) {
    if ('>' == c && '?' == last) {
      fprintf(c_file, "\nreturn 0;}");
      fclose(c_file);
      run_c_file(tmp_file_path);
      goto cleanup;
    } else if ('?' == last) {
      fwrite(&last, 1, 1, c_file);
    }
    if ('?' != c) {
      fwrite(&c, 1, 1, c_file);
    }

    last = c;
  }

  fclose(c_file);

cleanup:
  free(tmp_file_path);
}

void htmc_find_segments(FILE *f, const char *file_path) {
  char c;
  char last         = 0;
  bool found_open   = false;
  bool stop_emitter = false;
  int  line         = 1;
  while (0 != fread(&c, 1, 1, f)) {
    if ('<' == c) {
      stop_emitter = true;
    }
    if ('>' == c && !found_open) {
      stop_emitter = false;
    }
    if ('?' == c && '<' == last) {
      found_open = true;
    } else if ('<' == last) {
      stop_emitter = false;
      putchar(last);
    }
    if (!stop_emitter) {
      putchar(c);
    }
    if ('\n' == c) {
      line++;
    }
    if (found_open && 'c' == last && '>' == c) {
      parse_and_run(f, file_path, line);
      found_open   = false;
      stop_emitter = false;
    }
    last = c;
  }
}

int main(int argc, char *argv[]) {
  const char *file_path = "examples/test.htmc";
  if (argc > 2) {
    file_path = argv[1];
  }

  FILE *f = fopen(file_path, "r");
  htmc_find_segments(f, file_path);
  fclose(f);
}
