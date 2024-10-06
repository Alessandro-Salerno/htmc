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

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "emit.h"
#include "log.h"
#include "parse.h"

#define C_STR_CHAR          '"'
#define C_ESCAPE_CHAR       '\\'
#define C_LSCOPE_CHAR       '{'
#define C_RSCOPE_CHAR       '}'
#define C_DELIM             ';'
#define C_LINE_COMMENT_CHAR '/'
#define C_ML_COMMENT_CHAR2  '*'

#define COMMON_EOL_CHAR '\n'
#define COMMON_TAB_CHAR '\t'

#define IS_STR_DELIM(c)       C_STR_CHAR == c
#define IS_ESCAPE(c)          C_ESCAPE_CHAR == c
#define IS_SCOPE(c)           C_RSCOPE_CHAR == c
#define IS_EOS(c)             C_RSCOPE_CHAR == c
#define IS_DELIM(c)           C_DELIM == c
#define IS_EOL(c)             COMMON_EOL_CHAR == c
#define IS_TAB(c)             COMMON_TAB_CHAR == c
#define IS_LINE_COMMENT(l, c) (C_LINE_COMMENT_CHAR == c && c == l)
#define IS_ML_COMMENT(l, c) \
  (C_LINE_COMMENT_CHAR == l && C_ML_COMMENT_CHAR2 == c)
#define IS_ML_COMMNENT_END(l, c) \
  (C_ML_COMMENT_CHAR2 == l && C_LINE_COMMENT_CHAR == c)

#define IS_TAG_HTMC(c)  ('c' == c)
#define IS_TAG_CLOSE(c) ('>' == c)
#define IS_TAG_FIT(c)   ('?' == c)

typedef struct {
  uint64_t lineno;
  uint64_t chr_index;
  uint64_t scope_sum;
} parse_status_t;

inline void reset_parse_status(parse_status_t *parse_status) {
  parse_status->lineno++;
  parse_status->chr_index = 0;
}

bool find_tag_and_emit(FILE           *src_file,
                       FILE           *dst_file,
                       parse_status_t *parse_status) {
  bool found;
  char c;

  emit_html_base(dst_file);
  while ((found = !feof(src_file)) && '<' != (c = fgetc(src_file))) {
    emit_char_escaped(dst_file, c);
    if (IS_EOL(c)) {
      reset_parse_status(parse_status);
    }
  }

  emit_html_end(dst_file);
  return found;
}

bool is_tag_fit(FILE *src_file, char *cp) {
  if (feof(src_file)) {
    return false;
  }

  return IS_TAG_FIT((*cp = fgetc(src_file)));
}

bool is_tag_htmc(FILE *src_file, char *cp) {
  if (feof(src_file)) {
    return false;
  }

  return IS_TAG_HTMC((*cp = fgetc(src_file)));
}

bool collect_emit_c(FILE           *src_file,
                    FILE           *dst_file,
                    parse_status_t *parse_status) {
  bool escape       = false;
  bool string       = false;
  bool terminated   = false;
  bool line_comment = false;
  bool ml_comment   = false;
  bool comment      = line_comment || ml_comment;
  char last         = 0;
  char c            = 0;

  while ((c = fgetc(src_file))) {
    if (!string && IS_LINE_COMMENT(last, c)) {
      line_comment = true;
      goto emit;
    }

    if (!string && IS_ML_COMMENT(last, c)) {
      ml_comment = true;
      goto emit;
    }

    if (IS_EOL(c)) {
      string       = false;
      line_comment = false;
      reset_parse_status(parse_status);
      goto emit;
    }

    if (IS_ML_COMMNENT_END(last, c)) {
      ml_comment = false;
      goto emit;
    }

    // If there's a backslash in a string
    if (string && !escape && IS_ESCAPE(c)) {
      escape = true;
      goto emit;
    }

    // If there is a quote sign inside a string
    // preceded by an escpae backslash
    if (string && escape && IS_STR_DELIM(c)) {
      escape = false;
      goto emit;
    }

    // If char is " string might begin or end
    if (!escape && !comment && IS_STR_DELIM(c)) {
      string = !string;
      goto emit;
    }

    // Error if code contains end of main scope
    if (!string && IS_EOS(c) && 0 == parse_status->scope_sum) {
      return false;
    }

    // If char is ';'
    if (!string && !comment && IS_DELIM(c)) {
      terminated = true;
      goto emit;
    }

    // Update terminated status otherwise
    // terminated = terminated && (c < 33 || IS_TAG_HTMC(c));

    // if the code has been correctly terminated
    if (terminated && IS_TAG_FIT(last) && IS_TAG_CLOSE(c)) {
      return true;
    }

    // Update scope sum
    parse_status->scope_sum += IS_SCOPE(c);
    parse_status->scope_sum -= IS_EOS(c);

    escape = false;
    string = IS_STR_DELIM(c);

    if (IS_TAG_FIT(last)) {
      emit_char(dst_file, last);
    }
    if (!IS_TAG_FIT(c)) {
    emit:
      emit_char(dst_file, c);
    }
    // Update comment status
    comment = line_comment || ml_comment;
    last    = c;
  }

  return false;
}

int parse_and_emit(FILE *src_file, FILE *dst_file) {
  parse_status_t parse_status = {0};

  emit_base(dst_file);
  while (find_tag_and_emit(src_file, dst_file, &parse_status)) {
    char buf[2] = {0, 0};
    if (!is_tag_fit(src_file, &buf[0]) || !is_tag_htmc(src_file, &buf[1])) {
      emit_html_base(dst_file);
      emit_char_escaped(dst_file, '<');
      emit_char_escaped(dst_file, buf[0]);
      emit_char_escaped(dst_file, buf[1]);
      emit_html_end(dst_file);
      continue;
    }

    if (!collect_emit_c(src_file, dst_file, &parse_status)) {
      return -1;
    }
  }

  emit_end(dst_file);
  return 0;
}
