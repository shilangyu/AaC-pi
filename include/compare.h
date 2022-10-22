#include "args.h"
#include <stdint.h>
#pragma once

// Returns exit code.
int handle_compare(command_compare_t args);

// <-1: error:
//      -2: comparison error
//  -1: equals
// >-1: first diff index
int64_t compare(const char *file_name1, const char *file_name2);
