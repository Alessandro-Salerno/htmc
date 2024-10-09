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
