#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *get_command_name(command_type_t type) {
  switch (type) {
  case GENERATE:
    return "generate";
  case FIND:
    return "find";
  case COMPARE:
    return "compare";
  case TABLE:
    return "table";
  }
}

_Noreturn void print_help_and_exit(const char *program_name) {
  fprintf(stderr, "%s: pi generator, substring finder, and solution comparator.\n", program_name);
  fprintf(stderr, "Available commands:\n");
  fprintf(stderr, "\t%s %s [file] [n-digits]: generates #n-digits digits of pi into the specified file\n", program_name, get_command_name(GENERATE));
  fprintf(stderr, "\t%s %s [file] [substring]: finds the given substring in the given file and prints the starting index\n", program_name, get_command_name(FIND));
  fprintf(stderr, "\t%s %s [file1] [file2]: compares contents of two files and points to (if there is one) a difference\n", program_name, get_command_name(COMPARE));
  fprintf(stderr, "\t%s %s [pi_file] [out_file] [a] [b]: writes a table of substrings of natural numbers from a to b to n to out_file from the pi_file\n", program_name, get_command_name(TABLE));

  exit(1);
}

args_t parse_args(const char **argv, const size_t argc) {
  if (argc < 2) print_help_and_exit(argv[0]);

  const char *cmd = argv[1];
  command_type_t command_type;
  union args_value_t value;

  if (!strcmp(get_command_name(GENERATE), cmd)) {
    if (argc != 4) print_help_and_exit(argv[0]);

    command_type   = GENERATE;
    value.generate = (command_generate_t){.pi_file_path = argv[2], .n_digits = strtoul(argv[3], 0, 0)};
  } else if (!strcmp(get_command_name(FIND), cmd)) {
    if (argc != 4) print_help_and_exit(argv[0]);

    command_type = FIND;
    value.find   = (command_find_t){.pi_file_path = argv[2], .substring = argv[3]};
    if (strlen(value.find.substring) == 0) {
      print_help_and_exit(argv[0]);
    }
  } else if (!strcmp(get_command_name(COMPARE), cmd)) {
    if (argc != 4) print_help_and_exit(argv[0]);

    command_type  = COMPARE;
    value.compare = (command_compare_t){.file_path1 = argv[2], .file_path2 = argv[3]};
  } else if (!strcmp(get_command_name(TABLE), cmd)) {
    if (argc != 6) print_help_and_exit(argv[0]);

    uint64_t a = strtoul(argv[4], 0, 0);
    uint64_t b = strtoul(argv[5], 0, 0);

    if (a > b) print_help_and_exit(argv[0]);

    command_type = TABLE;
    value.table  = (command_table_t){.pi_file_path = argv[2], .out_file_path = argv[3], .a = a, .b = b};
  } else {
    print_help_and_exit(argv[0]);
  }

  return (args_t){
      .tag   = command_type,
      .value = value,
  };
}
