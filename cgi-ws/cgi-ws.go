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
  "bufio"
  "strings"
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
  if !createDirectory(BIN_DIRECTORY_PATH) || !createDirectory(INCLUDE_DIRECTORY_PATH) || !createDirectory(TMP_DIRECTORY_PATH) {
    return
  }

  if !exists(getHtmcExecPath()) || !exists(LIBHTMC_HEADER_PATH) || !exists(INDEX_HT_FILE) {
    fmt.Print("You're missing some important htmc files, proceed with the download? [Y/n]: ")
    reader := bufio.NewReader(os.Stdin)
    text, _ := reader.ReadString('\n')
    text = strings.Replace(text, "\n", "", -1)
    
    if (len(text) != 0 && text != "Y") {
      fmt.Println("\t1. Go to <https://github.com/Alessandro-Salerno/htmc> and download the latest release or latest-linux-bundle branch files")
      fmt.Println("\t2. Place the htmc executable (htmc or htmc.exe) in the bin/ directory")
      fmt.Println("\t3. Place the libhtmc.a file in the bin/ directory")
      fmt.Println("\t4. Copy the include/ directory in the working directory")
      fmt.Println("\t5. Make sure that include/libhtmc/libhtmc.h exists and is the libhtmc header file")
      fmt.Println("\t6. Make sure that GCC and GNU ld are installed")
      return
    }

    download(false, getHtmcExecPath(), "https://alessandro-salerno.github.io/htmc/bin/htmc")
    download(false, LIBHTMC_BINARY_PATH, "https://alessandro-salerno.github.io/htmc/bin/libhtmc.a")
    download(false, LIBHTMC_HEADER_PATH, "https://alessandro-salerno.github.io/htmc/include/libhtmc/libhtmc.h")
    download(false, INDEX_HT_FILE, "https://alessandro-salerno.github.io/htmc/examples/index.htmc")
    
    if !markFileAsExecutable(getHtmcExecPath()) {
      return
    }
  }

  // This is used as a state check for terminal output
  up_to_date := true

  if (checkUpdates()) {
    up_to_date = false

    fmt.Println()
    fmt.Println("Files to be updated:")
    fmt.Println("\t", getHtmcExecPath())
    fmt.Println("\t", LIBHTMC_BINARY_PATH)
    fmt.Println("\t", LIBHTMC_HEADER_PATH)
    fmt.Println()
    fmt.Print(":: Proceed with the installation? [Y/n]: ")
    reader := bufio.NewReader(os.Stdin)
    text, _ := reader.ReadString('\n')
    text = strings.Replace(text, "\n", "", -1)
    
    if (len(text) == 0 || text == "Y") {
      download(true, getHtmcExecPath(), "https://alessandro-salerno.github.io/htmc/bin/htmc")
      download(true, LIBHTMC_BINARY_PATH, "https://alessandro-salerno.github.io/htmc/bin/libhtmc.a")
      download(true, LIBHTMC_HEADER_PATH, "https://alessandro-salerno.github.io/htmc/include/libhtmc/libhtmc.h")

      if !markFileAsExecutable(getHtmcExecPath()) {
        return
      }

      up_to_date = true
    }
  }

  if up_to_date {
    fmt.Println()
    fmt.Println("All up to date")
  }

  fmt.Println()
  fmt.Println("Listening on localhost:80")
  fmt.Println()

  http.HandleFunc("/", htmcCGI)
  http.ListenAndServe("localhost:80", nil)
}
