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
  }
}

__attribute__((noreturn)) void print_help_and_exit(const char *program_name) {
  fprintf(stderr, "%s: π generator, substring finder, and solution comparator.\n", program_name);
  fprintf(stderr, "Available commands:\n");
  fprintf(stderr, "\t%s %s [file]: generates digits of π into the specified file\n", program_name, get_command_name(GENERATE));
  fprintf(stderr, "\t%s %s [file] [substring]: finds the given substring in the given file and prints the starting index\n", program_name, get_command_name(FIND));
  fprintf(stderr, "\t%s %s [file1] [file2]: compares contents of two files and points to (if there is one) a difference\n", program_name, get_command_name(COMPARE));

  exit(1);
}

args_t parse_args(const char **argv, const size_t argc) {
  if (argc < 2) print_help_and_exit(argv[0]);

  const char *cmd = argv[1];
  command_type_t command_type;
  union args_value_t value;

  if (!strcmp(get_command_name(GENERATE), cmd)) {
    if (argc != 3) print_help_and_exit(argv[0]);

    command_type   = GENERATE;
    value.generate = (command_generate_t){.pi_file_path = argv[2]};
  } else if (!strcmp(get_command_name(FIND), cmd)) {
    if (argc != 4) print_help_and_exit(argv[0]);

    command_type = FIND;
    value.find   = (command_find_t){.pi_file_path = argv[2], .substring = argv[3]};
  } else if (!strcmp(get_command_name(COMPARE), cmd)) {
    if (argc != 4) print_help_and_exit(argv[0]);

    command_type  = COMPARE;
    value.compare = (command_compare_t){.file_path1 = argv[2], .file_path2 = argv[3]};
  } else {
    print_help_and_exit(argv[0]);
  }

  return (args_t){
      .tag   = command_type,
      .value = value,
  };
}
