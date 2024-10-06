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
  return vprintf(fmt, args);
}

int impl_debug_query_vscanf(htmc_handover_t *handover,
                            const char      *fmt,
                            va_list          args) {
  if (0 == handover->query_param_sep_off) {
    handover->query_has_params = false;
    for (const char *cp = handover->query_string; *cp; cp++) {
      if ('?' == *cp) {
        handover->query_has_params = true;
        break;
      }

      handover->query_param_sep_off++;
    }
  }

  if (!handover->query_has_params) {
    return -1;
  }

  int fmt_off = 0;
  for (const char *cp = handover->query_string + handover->query_param_sep_off;
       *cp && fmt[fmt_off];
       cp++) {
    const char query_char = *cp;
    const char fmt_char   = fmt[fmt_off];

    if ('%' == fmt_char) {
      const char fmt_id = fmt[fmt_off + 1];

      // Temporary
      // Idea:
      // Create 4 char buffer
      // Copy %d/%s identifier into that buffer
      // use that string as fmt for sscanf
      switch (fmt_id) {
      case 'i':
      case 'd': {
        int *dst = va_arg(args, int *);
        sscanf(cp, "%d", dst);
        break;
      }

      case 'f':
        break;

      case 'c':
        break;

      case 's':
        break;
      }
      break;
    }

    if (query_char == fmt_char) {
      fmt_off++;
      continue;
    }

    fmt_off = 0;
  }

  return 0;
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
