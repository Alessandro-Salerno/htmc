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
  "os"
  "os/exec"
  "io"
)

func checkUpdates() bool {
  fmt.Println("Checking for updates...")

  output_bytes, err := exec.Command("./bin/htmc", "--version").Output();
  if err != nil {
    fmt.Println("Error while checking for updates");
    return false;
  }

  cur_v := string(output_bytes)

  resp, err := http.Get("https://raw.githubusercontent.com/Alessandro-Salerno/htmc/refs/heads/latest-linux-bundle/.htmc-version")
  if err != nil {
    fmt.Println("Error while requesting new version string")
    return false
  }
  defer resp.Body.Close()

  if resp.StatusCode != http.StatusOK {
    fmt.Println("Error while requesting new version string")
    return false
  }

  body_bytes, err := io.ReadAll(resp.Body)
  if err != nil {
    fmt.Println("Error while reading server response")
    return false
  }

  new_v := string(body_bytes)

  fmt.Print("Installed: ", cur_v)
  fmt.Print("Latest: ", new_v)

  return cur_v != new_v
}

func download(force bool, filepath string, url string) {
  if (exists(filepath) && !force) {
    fmt.Println("Ignoring downlaod of <", url, "> because ", filepath, " already exists");
    return
  }

  fmt.Print("Downloading <", url, "> to ", filepath, " ... ")

  out, err := os.Create(filepath)
  if err != nil  {
    fmt.Println("IO Error")
    return
  }
  defer out.Close()

  resp, err := http.Get(url)
  if err != nil {
    fmt.Println("Request Error")
    return
  }
  defer resp.Body.Close()

  if resp.StatusCode != http.StatusOK {
    fmt.Println("Request Status Error")
    return
  }

  _, err = io.Copy(out, resp.Body)
  if err != nil  {
    fmt.Println("IO Error")
    return
  }
  fmt.Println("Done!")
}

