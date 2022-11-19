# $\pi$ generation

[![](https://github.com/shilangyu/AaC-pi/workflows/cd/badge.svg)](https://github.com/shilangyu/AaC-pi/actions)

Repository for the $\pi$ generation project for the Algorithms and Computability course at the MiNI faculty.

## Building

### POSIX systems

Download the source and build the tool with `make release`. You will find the executable at `build/apps/pi`. Depends on a C compiler (setup for `clang`, but can be replaced with `gcc`) and the [`gmp`](https://gmplib.org) library (on MacOS, download `gmp` from brew: `brew install gmp`). Platform support will mostly depend on your compiler (32/64bit x86/arm should work).

### Windows

Download the source and open the project in Visual Studio, do not use the `Makefile`. Build with the release profile. You will find the executable in `build/x64/Release/aac-pi.exe`. No external dependencies are needed: [a fork of GMP called MPIR](http://www.mpir.org) was manually compiled as a static library and [vendored to polyfill](include/mpir_windows) the GMP code. Platform support is limited to 64bit x86 only, since this is the only version of MPIR I compiled.

## Documentation

Hosted [here](https://shilangyu.dev/AaC-pi/documentation.pdf). To build documentation you will need to install [`pandoc`](https://pandoc.org/) and then run `make build doc`. You will find the generated pdf at `build/documentation.pdf`.

## Usage

You can start by downloading a sample $\pi$ digits file with the `./script/download-baseline.sh` script.

```
./pi: π generator, substring finder, and solution comparator.
Available commands:
        ./pi generate [file] [n-digits]: generates #n-digits digits of π into the specified file
        ./pi find [file] [substring]: finds the given substring in the given file and prints the starting index
        ./pi compare [file1] [file2]: compares contents of two files and points to (if there is one) a difference
        ./pi table [pi_file] [out_file] [n]: writes a table of substrings of natural numbers up to n to out_file from the pi_file
```
