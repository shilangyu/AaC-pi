#include "table.h"
#include "find.h"
#include "main.h"
#include "mmap.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

void write_result(FILE *out, size_t number, int64_t offset) {
  if (offset == -1) {
    fprintf(out, "%zu,not found", number);
  } else {
    fprintf(out, "%zu,%" PRId64 "\n", number, offset);
  }
}

int handle_table(command_table_t args) {
  // memory map the whole file
  int fd = open(args.pi_file_path, O_RDONLY);
  if (fd == -1) ERR(args.pi_file_path);

  struct stat stats;
  if (fstat(fd, &stats) == -1) {
    CHECK(close(fd));
    ERR("fstat");
  }

  uint8_t *data = mmap(NULL, stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data == MAP_FAILED) {
    CHECK(close(fd));
    ERR("mmap");
  }

  FILE *out = fopen(args.out_file_path, "w");
  if (out == NULL) ERR(args.out_file_path);

  // figure out how many places are needed for a number
  const size_t len = snprintf(NULL, 0, "%" PRId64, args.n) + 1;
  char substr[len];
  for (size_t i = 0; i <= args.n; i++) {
    snprintf(substr, len, "%zu", i);
    int64_t offset = kmp(data, stats.st_size, substr);

    write_result(out, i, offset);
  }

  CHECK(munmap(data, stats.st_size));
  CHECK(close(fd));
  CHECK(fclose(out));

  return EXIT_SUCCESS;
}
