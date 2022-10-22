#include "find.h"
#include "main.h"
#include <fcntl.h>
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
int64_t kmp(const char *file_path, const char *substring) {
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

  // run KMP against the file by memory mapping the whole file

  int fd = open(file_path, O_RDONLY);
  if (fd == -1) ERR(file_path);

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

  size_t p = 0;
  off_t i  = 0;
  while (i < stats.st_size) {
    if (substring[p] == data[i]) {
      i += 1;
      p += 1;
    }

    if (p == sub_len) {
      return i - sub_len;
    } else if (i < stats.st_size && substring[p] != data[i]) {
      if (p == 0) {
        i += 1;
      } else {
        p = lps[p - 1];
      }
    }
  }

  CHECK(munmap(data, stats.st_size));
  CHECK(close(fd));

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
}

int handle_find(command_find_t args) {
  int64_t offset = kmp(args.pi_file_path, args.substring);

  if (offset == -1) {
    printf("Substring not found in the pointed file.\n");
    return EXIT_SUCCESS;
  }

  printf("Found substring in the given file at position %ld:\n", offset);
  print_sub(args.pi_file_path, offset, strlen(args.substring));

  return EXIT_SUCCESS;
}
