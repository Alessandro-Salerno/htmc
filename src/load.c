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
#include <stdlib.h>

#include "libhtmc/libhtmc.h"
#include "load.h"

void *load_htmc_so(const char *so_file_path) {
  if (!so_file_path) {
    return NULL;
  }

  return dlopen(so_file_path, RTLD_LAZY);
}

htmc_entry_point_t get_htmc_entry_point(void *so_handle) {
  if (!so_handle) {
    return NULL;
  }

  return (htmc_entry_point_t)dlsym(so_handle, HTMC_ENTRY_POINT_SYM);
}

int call_htmc_entry(htmc_entry_point_t entry_point, htmc_handover_t *handover) {
  if (!entry_point) {
    return EXIT_FAILURE;
  }

  return entry_point(handover);
}

int run_htmc_so(const char *so_file_path, htmc_handover_t *handover) {
  return call_htmc_entry(get_htmc_entry_point(load_htmc_so(so_file_path)),
                         handover);
}
