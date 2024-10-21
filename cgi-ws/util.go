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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

package main

import (
  "runtime"
  "fmt"
  "os"
  "os/exec"
)

const (
  BIN_DIRECTORY_PATH     = "./bin/"
  TMP_DIRECTORY_PATH     = "./tmp/"
  INCLUDE_DIRECTORY_PATH = "./include/libhtmc/"
  DOCUMENT_ROOT_PATH     = "./"
  INDEX_HT_FILE          = DOCUMENT_ROOT_PATH + "index.htmc"
  HTMC_EXEC_BASE_PATH    = BIN_DIRECTORY_PATH + "htmc"
  LIBHTMC_BINARY_PATH    = BIN_DIRECTORY_PATH + "libhtmc.a"
  LIBHTMC_HEADER_PATH    = INCLUDE_DIRECTORY_PATH + "libhtmc.h"
)

func getHtmcExecPath() string {
  if runtime.GOOS == "windows" {
    return HTMC_EXEC_BASE_PATH + ".exe"
  }

  return HTMC_EXEC_BASE_PATH
}

func createDirectory(directory string) bool {
  if !exists(directory) {
    err := os.MkdirAll(directory, 0755)
    if err != nil {
      fmt.Println("Unable to create ", directory, " directory")
      return false
    }
  }

  return true
}

func markFileAsExecutable(filepath string) bool {
  if runtime.GOOS == "linux" {
    if exec.Command("chmod", "+x", filepath).Run() != nil {
      fmt.Println("Error while setting permissions for ./bin/htmc")
      return false
    }
  }

  return true
}
