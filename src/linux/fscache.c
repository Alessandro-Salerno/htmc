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

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif

#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "fscache.h"

double fscache_cmp_ff(FILE *f1, FILE *f2) {
  struct stat f1_stat;
  struct stat f2_stat;

  int stat1_ret = fstat(fileno(f1), &f1_stat);
  int stat2_ret = fstat(fileno(f1), &f2_stat);

  if (0 != stat1_ret || 0 != stat2_ret) {
    return 0;
  }

  return difftime(f1_stat.st_mtime, f2_stat.st_mtime);
}

double fscache_cmp_pp(const char *p1, const char *p2) {
  struct stat f1_stat;
  struct stat f2_stat;

  int stat1_ret = stat(p1, &f1_stat);
  int stat2_ret = stat(p2, &f2_stat);

  if (0 != stat1_ret || 0 != stat2_ret) {
    return 0;
  }

  return difftime(f1_stat.st_mtime, f2_stat.st_mtime);
}

double fscache_cmp_fp(FILE *f1, const char *f2) {
  struct stat f1_stat;
  struct stat f2_stat;

  int stat1_ret = fstat(fileno(f1), &f1_stat);
  int stat2_ret = stat(f2, &f2_stat);

  if (0 != stat1_ret || 0 != stat2_ret) {
    return 0;
  }

  return difftime(f1_stat.st_mtime, f2_stat.st_mtime);
}
