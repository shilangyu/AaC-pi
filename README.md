# $\pi$ generation

Repository for the $\pi$ generation project for the Algorithms and Computability course at the MiNI faculty.

## Building

Download the source and build the tool with `make release`. You will find the executable at `build/apps/pi`. Depends on a C compiler (setup for `clang`, but can be replaced with `gcc`) and the [`gmp`](https://gmplib.org) library (on MacOS, download `gmp` from brew: `brew install gmp`).

To build documentation you will need to install [`pandoc`](https://pandoc.org/) and then run `make doc`. You will find the generated pdf at `build/documentation.pdf`.

## Usage

You can start by downloading a sample $\pi$ digits file with the `./download-baseline.sh` script.

```
./build/apps/pi: π generator, substring finder, and solution comparator.
Available commands:
        ./build/apps/pi generate [file]: generates digits of π into the specified file
        ./build/apps/pi find [file] [substring]: finds the given substring in the given file and prints the starting index
        ./build/apps/pi compare [file1] [file2]: compares contents of two files and points to (if there is one) a difference
```
