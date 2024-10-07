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
The easiest (and slowest) way to use htmc is

_TO BE CONTINUED_

# License
htmc is distributed under the terms of the MIT license. See [LICENSE](LICENSE) for more information.
