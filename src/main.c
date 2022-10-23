#include "args.h"
#include "compare.h"
#include "find.h"
#include "generate.h"
#include <stdlib.h>
#include <stdio.h>

int main(const int argc, const char **argv) {
  args_t args = parse_args(argv, argc);

  switch (args.tag) {
  case GENERATE:
    return handle_generate(args.value.generate);
  case FIND:
    return handle_find(args.value.find);
  case COMPARE:
    return handle_compare(args.value.compare);
  }

  fprintf(stderr, "Unhandled command\n");
  return 1;
}
