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
