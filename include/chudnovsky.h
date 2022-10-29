#include <stdint.h>
#include <stdio.h>
#pragma once

// Writes n_digits of π into output_file
void chudnovsky(FILE *output_file, uint64_t n_digits);
