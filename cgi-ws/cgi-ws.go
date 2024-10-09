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
      fmt.Print("Unable to create bin/ directory")
      return
    }
  }

  if !exists("./tmp") {
    err := os.MkdirAll("./tmp", 0755)
    if err != nil {
      fmt.Print("Unable to create tmp/ directory")
      return
    }
  }

  if !exists("./htdocs") {
    err := os.MkdirAll("./htdocs", 0755)
    if err != nil {
      fmt.Print("Unable to create htdocs/ directory")
      return
    }
  }

  http.HandleFunc("/", htmcCGI)
  http.ListenAndServe("localhost:80", nil)
}
