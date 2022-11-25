#include <stddef.h>
#include <stdint.h>
#pragma once

typedef enum {
  GENERATE,
  COMPARE,
  FIND,
  TABLE,
} command_type_t;

typedef struct {
  const char *pi_file_path;
  uint64_t n_digits;
} command_generate_t;

typedef struct {
  const char *file_path1;
  const char *file_path2;
} command_compare_t;

typedef struct {
  const char *substring;
  const char *pi_file_path;
} command_find_t;

typedef struct {
  const char *pi_file_path;
  const char *out_file_path;
  uint64_t a;
  uint64_t b;
} command_table_t;

// Tagged union of possible CLI commands
typedef struct {
  const command_type_t tag;
  const union args_value_t {
    command_generate_t generate;
    command_compare_t compare;
    command_find_t find;
    command_table_t table;
  } value;
} args_t;

// Parses command line arguments into a resolved command.
// If arguments are wrong, prints help and exits the program.
args_t parse_args(const char **argv, const size_t argc);
