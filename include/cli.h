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

#pragma once

#include <stdbool.h>

typedef struct {
  const char *input_file;
  const char *output_path;
  bool        stop_splash;
  bool        log_level_set;
} cli_info_t;

typedef int (*cli_fcn_t)(cli_info_t *info, const char *next);
typedef int (*cli_exec_t)(cli_info_t info);

typedef struct {
  const char *short_option;
  const char *full_option;
  cli_fcn_t   handler;
  bool        has_argument;
  cli_exec_t  exec_handler;
} cli_opt_desc_t;

// Support functions
void print_program_version();
void print_program_info();

// Handler functions (cli_fcn_t)
int flag_no_splash(cli_info_t *info, const char *next);
int flag_output(cli_info_t *info, const char *next);
int flag_log_level(cli_info_t *info, const char *next);

// Setup for executable functions
int setup_cli_version(cli_info_t *info, const char *next);

// Executable functions (cli_exec_t)
int cli_help(cli_info_t info);
int cli_license(cli_info_t info);
int cli_version(cli_info_t info);
int cli_translate(cli_info_t info);
int cli_compile(cli_info_t info);
int cli_run(cli_info_t info);
int cli_load_shared(cli_info_t info);
int cli_run(cli_info_t info);
