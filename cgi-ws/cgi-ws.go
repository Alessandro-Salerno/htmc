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
  "fmt"
  "net/http"
  "net/http/cgi"
  "os"
)

func htmcCGI(w http.ResponseWriter, r *http.Request) {
  handler := cgi.Handler{Path: "./bin/htmc", Dir: "./", Args: []string{"-ll", "off", "-ns"}}
  handler.ServeHTTP(w, r)
}

func exists(path string) (bool) {
  _, err := os.Stat(path)
  return err == nil
}

func main() {
  if !exists("./bin") {
    err := os.MkdirAll("./bin", 0755)
    if err != nil {
      fmt.Println("Unable to create bin/ directory")
      return
    }
  }

  if !exists("./tmp") {
    err := os.MkdirAll("./tmp", 0755)
    if err != nil {
      fmt.Println("Unable to create tmp/ directory")
      return
    }
  }

  if !exists("./htdocs") {
    err := os.MkdirAll("./htdocs", 0755)
    if err != nil {
      fmt.Println("Unable to create htdocs/ directory")
      return
    }
  }

  if (!exists("./bin/htmc") && !exists("./bin/htmc.exe")) || !exists("./bin/libhtmc.a") || !exists("./include/libhtmc/libhtmc.h") {
    fmt.Println("You're missing some important htmc files:")
    fmt.Println("\t1. Go to <https://github.com/Alessandro-Salerno/htmc> and download the latest release files")
    fmt.Println("\t2. Place the htmc executable (htmc or htmc.exe) in the bin/ directory")
    fmt.Println("\t3. Place the libhtmc.a file in the bin/ directory")
    fmt.Println("\t4. Copy the include/ directory in the working directory")
    fmt.Println("\t5. Make sure that include/libhtmc/libhtmc.h exists and is the libhtmc header file")
    fmt.Println("\t6. Make sure that GCC and GNU ld are installed")
    return
  }

  http.HandleFunc("/", htmcCGI)
  http.ListenAndServe("localhost:80", nil)
}
