#include "find.h"
#include "main.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// Knuth-Morris-Pratt pattern search algorithm
//  -1: substring not found
// >-1: index of the start of the found substring
int64_t kmp(const uint8_t *data, const size_t data_len, const char *substring) {
  const size_t sub_len = strlen(substring);

  // create the longest prefix suffix table
  size_t lps[sub_len];
  lps[0] = 0;

  {
    size_t p = 0;
    size_t i = 1;
    while (i < sub_len) {
      if (substring[i] == substring[p]) {
        lps[i++] = ++p;
      } else if (p == 0) {
        lps[i++] = 0;
      } else {
        p = lps[p - 1];
      }
    }
  }

  // run KMP against the data

  size_t p = 0;
  size_t i = 0;
  while (i < data_len) {
    if (substring[p] == data[i]) {
      i += 1;
      p += 1;
    }

    if (p == sub_len) {
      return i - sub_len;
    } else if (i < data_len && substring[p] != data[i]) {
      if (p == 0) {
        i += 1;
      } else {
        p = lps[p - 1];
      }
    }
  }

  return -1;
}

int64_t naive(const uint8_t *data, const size_t data_len, const char *substring) {
  const size_t sub_len = strlen(substring);

  for (size_t i = 0; i < data_len - sub_len; i++) {
    bool found = true;
    for (size_t j = 0; j < sub_len; j++) {
      if (data[i + j] != substring[j]) {
        found = false;
        break;
      }
    }

    if (found) {
      return i;
    }
  }

  return -1;
}

void print_sub(const char *file_path, int64_t offset, size_t length) {
  const size_t window = 20;
  uint8_t buf[window + length];

  FILE *file = fopen(file_path, "r");
  if (file == NULL) ERR(file_path);

  size_t move = MAX(0, offset - (int64_t)window / 2);

  CHECK(fseek(file, move, SEEK_SET));

  size_t read = fread(buf, 1, window + length, file);

  if (move != 0)
    printf("…");
  for (size_t i = 0; i < offset - move; i++)
    printf("%c", buf[i]);
  printf("\x1b[1;32m");
  for (size_t i = 0; i < length; i++)
    printf("%c", buf[offset - move + i]);
  printf("\x1b[0m");
  for (size_t i = offset - move + length; i < read; i++)
    printf("%c", buf[i]);
  printf("…\n");

  CHECK(fclose(file));
}

int handle_find(command_find_t args) {
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

  int64_t offset = kmp(data, stats.st_size, args.substring);

  if (offset == -1) {
    printf("Substring not found in the pointed file.\n");
  } else {
    printf("Found substring in the given file at position %" PRId64 ":\n", offset);
    print_sub(args.pi_file_path, offset, strlen(args.substring));
  }

  CHECK(munmap(data, stats.st_size));
  CHECK(close(fd));

  return EXIT_SUCCESS;
}
