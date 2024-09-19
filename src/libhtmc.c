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

#include <stdint.h>

#include "libhtmc/libhtmc.h"

static htmc_handover_t *targetHandover;

void htmc_bind(htmc_handover_t *handover) {
  targetHandover = handover;
}

int htmc_printf(const char *fmt, ...) {
  return 0;
}

int htmc_vprintf(const char *fmt, va_list args) {
  return targetHandover->vprintf(targetHandover, fmt, args);
}

int htmc_query_scanf(const char *fmt, ...) {
  return 0;
}

int htmc_query_vscanf(const char *fmt, va_list args) {
  return targetHandover->query_vscanf(targetHandover, fmt, args);
}

int htmc_form_scanf(const char *fmt, ...) {
  return 0;
}

int htmc_form_vscafn(const char *fmt, va_list args) {
  return targetHandover->form_vscanf(targetHandover, fmt, args);
}

void htmc_error(const char *fmt, ...) {
}

void htmc_verror(const char *fmt, va_list args) {
}

// Implementation:
// Terminal debugger

static uint8_t debugStack[1024]     = {0};
static size_t  debugStackOffset     = 0;
static size_t  debugStackLastOffset = 0;

static int
impl_debug_vprintf(htmc_handover_t *handover, const char *fmt, va_list args) {
  debugStackLastOffset = debugStackOffset;
  // TODO: Implement stuff with format strings
  return -1;
}

static int impl_debug_query_vscanf(htmc_handover_t *handover,
                                   const char      *fmt,
                                   va_list          args) {
  // TODO: implement stuff with format name=%d etc etc
  return -1;
}

static int impl_debug_form_vscanf(htmc_handover_t *handover,
                                  const char      *fmt,
                                  va_list          args) {
  // TODO: implement stuff with format name=%d etc etc
  return -1;
}

static void *impl_debug_alloc(htmc_handover_t *handover, size_t nbytes) {
  if (debugStackOffset >= sizeof debugStack) {
    return NULL;
  }

  nbytes += nbytes % 16;
  size_t offset = debugStackOffset;
  debugStackOffset += nbytes;
  return &debugStack[offset];
}

static void impl_debug_free(htmc_handover_t *handover, void *ptr) {
  htmc_error(
      "Internal htmc error: free(size_t *ptr) not supported in debug mode.");
}

static void impl_debug_cleanup(htmc_handover_t *handover) {
  debugStackOffset = debugStackLastOffset;
}
