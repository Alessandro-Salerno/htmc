[contributors-shield]: https://img.shields.io/github/contributors/Alessandro-Salerno/htmc.svg?style=flat-square
[contributors-url]: https://github.com/Alessandro-Salerno/htmc/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/Alessandro-Salerno/htmc.svg?style=flat-square
[forks-url]: https://github.com/Alessandro-Salerno/htmc/network/members
[stars-shield]: https://img.shields.io/github/stars/Alessandro-Salerno/htmc.svg?style=flat-square
[stars-url]: https://github.com/Alessandro-Salerno/htmc/stargazers
[issues-shield]: https://img.shields.io/github/issues/Alessandro-Salerno/htmc.svg?style=flat-square
[issues-url]: https://github.com/Alessandro-Salerno/htmc/issues
[license-shield]: https://img.shields.io/github/license/Alessandro-Salerno/htmc.svg?style=flat-square
[license-url]: https://github.com/Alessandro-Salerno/htmc/blob/master/LICENSE.txt

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
![](https://tokei.rs/b1/github/Alessandro-Salerno/htmc)

<br>
<br>

<p align="center">
    <img src=".github/htmc-logo.svg" height="120">
</p>

<br>

# What is htmc?
htmc is a tool that allows you to easily integrate C code in your HTML pages and vice versa. Files ending in `.htmc` can be used to describe the structure of a page statically in HTML and dynamically in C, much like other hypertex preprocessors such as [PHP](https://www.php.net/).  Regular C source files can also be compiled with htmc and used to generate fully dynamic web pages.

# Why use htmc?
htmc uses [GCC](https://gcc.gnu.org/) and the [GNU Linker](https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html) to generate highly optimized native binaries that can be cached and run on demande via htmc. **In theory**, this allows for much faster execution* compared to many common [interpreted languages](https://en.wikipedia.org/wiki/Interpreter_(computing)) like PHP, Python, and JavaScript. 

Writing web code in C can also be a fun challenge or academic exercise. Given enough tinkering, you can get htmc to do everything you need in a dynamic web page or HTTP API endpoint.

\* _The performance of code executed through htmc has not been tested yet. Also, most important optimizations have yet to be implemented as they stricly require htmc to integrate a web server of its own._

# Ways to use htmc
htmc is built to support multiple usecases. As of now, htmc can be used:
1. As a CLI tool
2. As a [CGI](https://it.wikipedia.org/wiki/Common_Gateway_Interface) script
3. As a static library (using headers from the `include` directory and `libhtmc.a`)

The recommended mode for serving web content is CGI as it allows for easy integration with existing web software.

## Integrated web server
CGI is known to be old and slow, hence why htmc will get its own web server at some point. The integrated web server will use preallocated [arenas](https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator), memory and network optimizations to maximize execution speed and minimize overhead, hopefully.

# How to use htmc easily


<details>
    <summary>Requirements</summary>

<br>
    
- Modern GCC Compiler or equivilent
- Decently recent version of the GNU Linker or equivilent
- GNU + Linux or other compatible Linux-based systems

**NOTE:** As of now replacing GCC or GNU Linker with other software is not encouraged. Some commandline options specified by htmc may be different in other compilers/linkers.

</details>

<details>
    <summary>How to use libhtmc</summary>

<br>

libhtmc contains all htmc functions. The library can be used in other native programs to integrate htmc, but also includes an interface to interact with the htmc runtime and manipulate HTML code

| Function interface | Description |
| - | - |
| `void  htmc_bind(htmc_handover_t *handover)` | Binds an `htmc_handover_t` pointer to the current htmc execution unit |
| `int   htmc_printf(const char *fmt, ...)` | Writes a formatted string to the HTML page |
| `int   htmc_vpprintf(const char *fmt, va_list args)` | Writes a formatted string to the HTML page |
| `int   htmc_query_scanf(const char *fmt, ...)` | Reads values from HTTP query arguments |
| `int   htmc_query_vscanf(const char *fmt, va_list args)` | Reads values from HTTP query arguments |
| `int   htmc_form_scanf(const char *fmt, ...)` | Reads values from HTTP body arguments in POST requests |
| `int   htmc_form_vscanf(const char *fmt, va_list args)`  | Reads values from HTTP body arguments in POST requests |
| `int   htmc_error(const char *fmt, ...)` | Throws a formatted error message |
| `void *htmc_alloc(size_t size)` | Returns a `void *` to a memory buffer of the requested size or `NULL` if it fails |
| `void  htmc_free(void *ptr)` | Frees a memory buffer allocated with `htmc_alloc` |

</details>


## CGI web server
The easiest (and slowest) way to use htmc is to create a simple CGI web server in a high level language and invoke htmc when handling request. In this example, [Golang](https://go.dev/) is used as it's one of the simplest native languages that supports these features out of the box. A Golang web server is included in this repository.

1. Download the `htmc-cgi-ws` from the [releases](/releases/)
2. Create a directory for the web server and place the `htmc-cgi-ws` executable inside of it
3. Write a script `launch-ws.sh` to launch `htmc-cgi-ws` as follows
```bash
#!/bin/sh
sudo ./htmc-cgi-ws
```
4. Launch `htmc-cgi-ws` with the script or by hand and follow the instructions
5. Write valid htmc code in `htdocs/index.htmc`
```html
<html>
    <head>
        <title>My htmc page</title>
    </head>

    <body>
        <?c
            htmc_printf("<p>This was generated with htmc</p>");
        ?>
    </body>
</html>
```
5. You should now be able to run the `launch-ws.sh` script and handle requests for htmc web pages

# How to build htmc

_TO BE CONTINUED_

# License
htmc is distributed under the terms of the MIT license. See [LICENSE](LICENSE) for more information.
