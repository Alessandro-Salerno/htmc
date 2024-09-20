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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "libhtmc/libhtmc-internals.h"
#include "libhtmc/libhtmc.h"

static htmc_handover_t *targetHandover;

void htmc_bind(htmc_handover_t *handover) {
  targetHandover = handover;
}

int htmc_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int r = htmc_vprintf(fmt, args);
  va_end(args);
  return r;
}

int htmc_vprintf(const char *fmt, va_list args) {
  return targetHandover->vprintf(targetHandover, fmt, args);
}

int htmc_query_scanf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int r = htmc_query_vscanf(fmt, args);
  va_end(args);
  return r;
}

int htmc_query_vscanf(const char *fmt, va_list args) {
  return targetHandover->query_vscanf(targetHandover, fmt, args);
}

int htmc_form_scanf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int r = htmc_form_vscafn(fmt, args);
  va_end(args);
  return r;
}

int htmc_form_vscafn(const char *fmt, va_list args) {
  return targetHandover->form_vscanf(targetHandover, fmt, args);
}

void *htmc_alloc(size_t nbytes) {
  return targetHandover->alloc(targetHandover, nbytes);
}

void htmc_free(void *ptr) {
  targetHandover->free(targetHandover, ptr);
}

void htmc_cleanup() {
  targetHandover->cleanup(targetHandover);
}

void htmc_error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  htmc_verror(fmt, args);
  va_end(args);
}

void htmc_verror(const char *fmt, va_list args) {
  htmc_vprintf(fmt, args);
}

// Implementation:
// Terminal debugger

static uint8_t debugStack[1024]     = {0};
static size_t  debugStackOffset     = 0;
static size_t  debugStackLastOffset = 0;

int impl_debug_vprintf(htmc_handover_t *handover,
                       const char      *fmt,
                       va_list          args) {
  debugStackLastOffset     = debugStackOffset;
  static char *str_num     = NULL;
  int          num_formats = 0;

  if (!str_num) {
    str_num = handover->alloc(handover, 16);
  }

  for (const char *cp = fmt; *cp; cp++) {
    if ('%' != *cp) {
      putchar(*cp);
      continue;
    }

    switch (*(++cp)) {
    case 'i':
    case 'd':
      sprintf(str_num, "%d", va_arg(args, int));
      printf("%s", str_num);
      num_formats++;
      break;

    case 'f':
      sprintf(str_num, "%f", (float)va_arg(args, int));
      printf("%s", str_num);
      num_formats++;
      break;

    case 'c':
      putchar((char)va_arg(args, int));
      num_formats++;
      break;

    case 's':
      printf("%s", va_arg(args, const char *));
      num_formats++;
      break;

    default:
      putchar('%');
      break;
    }
  }

  str_num[0] = 0;
  return num_formats;
}

int impl_debug_query_vscanf(htmc_handover_t *handover,
                            const char      *fmt,
                            va_list          args) {
  // TODO: implement stuff with format name=%d etc etc
  return -1;
}

int impl_debug_form_vscanf(htmc_handover_t *handover,
                           const char      *fmt,
                           va_list          args) {
  // TODO: implement stuff with format name=%d etc etc
  return -1;
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

void impl_debug_cleanup(htmc_handover_t *handover) {
  debugStackOffset = debugStackLastOffset;
}