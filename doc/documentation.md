---
title: $\pi$ project
subtitle: Algorithms & Computability course at the MiNI faculty
author:
  - Kamil Monicz (123456)
  - Marcin Wojnarowski (303880)
published: true
date: October, 2022
urlcolor: cyan
# geometry: margin=2cm
---

<!-- documentation of algorithms, including algorithms pseudocode, exhaustive description, and analysis of their complexity -->

\tableofcontents

# Description

The $\pi$ project for the Algorithms & Computability course consists of three components:

1. Generation of $\pi$ - program generating consecutive decimal digits of $\pi$ (ratio of a circle's circumference to its diameter) into a text file
2. Finding substrings - program finding a specified string of digits in a given file
3. Comparing files - program comparing two solutions and finding the difference

# Program

Our solution consists of a single program written in C which exposes required functionalities as subcommands. Namely:

\scriptsize

```
./pi: pi generator, substring finder, and solution comparator.
Available commands:
        ./pi generate [file]: generates digits of pi into the specified file
        ./pi find [file] [substring]: finds the given substring in the given file and prints the starting index
        ./pi compare [file1] [file2]: compares contents of two files and points to (if there is one) a difference
```

\normalsize

## Generate

<!-- TODO -->

## Find

<!-- TODO -->

## Compare

The task of comparing file contents is of a linear nature. The content of files is unstructured and no assumptions can be made about it, meaning no heuristics can be applied to improve the complexity of the comparison. We are thus stuck with $\mathcal O(\min(m, n))$ where $m$ and $n$ are lengths of the content of the compared files. However, one can take advantage of the available hardware to speed up the process.

Memory wise, it depends on the chosen buffer for file reading. Since it is known we will be working with large files, any buffer size depending on the input size would be a poor decision. Therefore we chose a constant size of $1024 \cdot 1024\text{B} = 1\text{MiB}$ for each file. This size was picked somewhat arbitrarily based on empirical tests.

### Algorithm description

In a loop each file is read into its appropriate buffer and then comparisons are made. For comparisons, we employ a simple trick to take advantage of the platform. Instead of comparing character by character (byte by byte) we compare by the word size of the underlying CPU. We transmute the memory of the byte buffer into a word buffer (few bytes combined, depends on the bit architecture of the CPU). Then, comparisons are done on this new buffer. This way, we cut down on the number of comparisons by a constant factor. For example, on a x86-64 CPU we perform 8 times less comparisons (64 bits $\to$ 8 bytes $\to$ one word). To illustrate this effect we can examine the generated assembly for a x86-64 CPU using the byte and word comparison (Fig. \ref{fig:byte_vs_word}). In practice this optimizations yielded on average $2.24$ times faster runtime.

![Generated x64 assembly for a byte and word comparison. The amount of instructions is the same, but byte comparison uses the `cmpb` instruction while word comparison uses `cmpq` which compares 8 byte registers. Source: [Compiler Explorer](https://godbolt.org/).\label{fig:byte_vs_word}](byte_vs_word.png)

Once a mismatch is found, the composed word is broken down into individual bytes to find the offending byte. Once found, the comparison is stopped and the found offset where the difference lies is returned. If no mismatch is found till the end of both files, program reports that no differences are found.

If we reach the end of one of the files, the program is stopped and the current offset is returned.

To sum up: $m + n$ reads are done and $\frac{\min(m, n)}{\text{CPU word size}}$ comparisons are done which means the algorithm time complexity is $\mathcal O(m + n + \frac{\min(m, n)}{\text{CPU word size}}) = \mathcal O(\min(m, n))$, linear.
