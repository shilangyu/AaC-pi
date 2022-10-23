#include "args.h"
#include <stdint.h>
#pragma once

// Returns exit code.
int handle_find(command_find_t args);

// Knuth-Morris-Pratt pattern search algorithm
//  -1: substring not found
// >-1: index of the start of the found substring
int64_t kmp(const char *file_path, const char *substring);
