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

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "libhtmc/libhtmc-internals.h"

static uint8_t debugStack[1024]     = {0};
static size_t  debugStackOffset     = 0;
static size_t  debugStackLastOffset = 0;

int impl_debug_vprintf(htmc_handover_t *handover,
                       const char      *fmt,
                       va_list          args) {
  return vprintf(fmt, args);
}

int impl_debug_puts(htmc_handover_t *handover, const char *s) {
  return fputs(s, stdout);
}

void *impl_debug_alloc(htmc_handover_t *handover, size_t nbytes) {
  if (debugStackOffset >= sizeof debugStack) {
    return NULL;
  }

  nbytes += nbytes % 16;
  size_t offset = debugStackOffset;
  debugStackOffset += nbytes;
  return &debugStack[offset];
}

void impl_debug_free(htmc_handover_t *handover, void *ptr) {
}
