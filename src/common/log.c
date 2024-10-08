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
#include <string.h>

#include "log.h"

const char *const HTMC_STR_LOG_LEVELS[] = {"all",
                                           "info",
                                           "warning",
                                           "error",
                                           "off"};

log_lvl_t logLevel;
bool      logSafeMode = false;

void log_fatal(const char *message) {
  if (HTMC_LOG_LEVEL_ERROR >= logLevel || !logSafeMode) {
    fprintf(stderr, "htmc fatal error: %s.\n", message);
  }
}

void log_error(const char *message) {
  if (HTMC_LOG_LEVEL_ERROR >= logLevel || !logSafeMode) {
    fprintf(stderr, "htmc error:       %s.\n", message);
  }
}

void log_info(const char *message) {
  if (HTMC_LOG_LEVEL_INFO >= logLevel || !logSafeMode) {
    fprintf(stderr, "htmc info:        %s.\n", message);
  }
}

void log_set_level(log_lvl_t lvl) {
  logLevel = lvl;
}

void log_set_safe() {
  logSafeMode = true;
}

int log_translate_level(const char *lvl_str) {
  for (int i = 0; i < sizeof HTMC_STR_LOG_LEVELS / sizeof(char *); i++) {
    if (0 == strcmp(lvl_str, HTMC_STR_LOG_LEVELS[i])) {
      return i;
    }
  }

  return -1;
}
