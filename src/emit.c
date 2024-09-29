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

#include <stdio.h>

#include "emit.h"

#define HTMC_C_BASE                        \
  "#include \"libhtmc/libhtmc.h\"\n\n"     \
  "void htmc_main(htmc_handover_t *h) {\n" \
  "htmc_bind(h);\n"

#define HTMC_C_BASE_END "}"

#define HTMC_HTML_BASE "htmc_printf(\""

#define HTMC_HTML_BASE_END "\");\n"

void emit_str(FILE *dst_file, const char *str) {
  fprintf(dst_file, "%s", str);
}

void emit_base(FILE *dst_file) {
  fprintf(dst_file, HTMC_C_BASE);
}

void emit_end(FILE *dst_file) {
  fprintf(dst_file, "%s", HTMC_C_BASE_END);
}

void emit_html_base(FILE *dst_file) {
  fprintf(dst_file, "%s", HTMC_HTML_BASE);
}

void emit_html_end(FILE *dst_file) {
  fprintf(dst_file, "%s", HTMC_HTML_BASE_END);
}

void emit_char(FILE *dst_file, char chr) {
  fputc(chr, dst_file);
}

void emit_char_escaped(FILE *dst_file, char chr) {
  switch (chr) {
  case '"':
    emit_str(dst_file, "\\\"");

  case '\\':
    emit_str(dst_file, "\\\\");

  case '\n':
    emit_str(dst_file, "\\n");

  case '\r':
    emit_str(dst_file, "\\r");

  case '\t':
    emit_str(dst_file, "    ");

  default:
    if (chr > 31) {
      emit_char(dst_file, chr);
    }
  }
}
