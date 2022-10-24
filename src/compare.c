#include "compare.h"
#include "main.h"
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMPARE_BUFFER_SIZE 1024 * 1024
typedef size_t compare_size_t;

void zero_rest(uint8_t *buff, size_t read) {
  int read_delta = COMPARE_BUFFER_SIZE - read;
  if (read_delta == 0)
    return;

  memset(buff + COMPARE_BUFFER_SIZE - read_delta, 0, read_delta);
}

// <-1: error:
//      -2: comparison error
//  -1: files are equal
// >-1: first diff index
int64_t compare(const char *file_name1, const char *file_name2) {
  // For this alg to work, we need to make sure the buffer size is divisible by the compare bit width
  assert(COMPARE_BUFFER_SIZE / sizeof(compare_size_t) * sizeof(compare_size_t) == COMPARE_BUFFER_SIZE);
  int64_t result = -1;

  uint8_t buff_1[COMPARE_BUFFER_SIZE];
  uint8_t buff_2[COMPARE_BUFFER_SIZE];

  compare_size_t *buff_1_64 = (compare_size_t *)buff_1;
  compare_size_t *buff_2_64 = (compare_size_t *)buff_2;

  FILE *file_1 = fopen(file_name1, "r");
  if (file_1 == NULL) ERR(file_name1);
  FILE *file_2 = fopen(file_name2, "r");
  if (file_2 == NULL) ERR(file_name2);

  for (int64_t iter = 0; result == -1; iter++) {
    size_t read_1 = fread(buff_1, 1, COMPARE_BUFFER_SIZE, file_1);
    size_t read_2 = fread(buff_2, 1, COMPARE_BUFFER_SIZE, file_2);

    zero_rest(buff_1, read_1);
    zero_rest(buff_2, read_2);

    for (size_t i = 0; i < COMPARE_BUFFER_SIZE / sizeof(compare_size_t); i++) {
      if (buff_1_64[i] == buff_2_64[i])
        continue;

      for (size_t j = i * sizeof(compare_size_t); j < (i + 1) * sizeof(compare_size_t); j++) {
        if (buff_1[j] != buff_2[j]) {
          result = j + iter * COMPARE_BUFFER_SIZE;
          break;
        }
      }

      if (result != -1) break;

      return -2;
    }

    // break if EOF
    if (read_1 < COMPARE_BUFFER_SIZE || read_2 < COMPARE_BUFFER_SIZE)
      break;
  }

  CHECK(fclose(file_1));
  CHECK(fclose(file_2));

  return result;
}

void print_diff(const char *file_path, int64_t diff) {
  const size_t window = 20;
  uint8_t buf[window];

  FILE *file = fopen(file_path, "r");
  if (file == NULL) ERR(file_path);

  size_t move = MAX(0, diff - (int64_t)window / 2);

  CHECK(fseek(file, move, SEEK_SET));

  size_t read = fread(buf, 1, window, file);

  printf("%s: ", file_path);
  if (move != 0) {
    printf("…");
  }

  printf("\x1b[32m");
  for (size_t i = 0; i < diff - move; i++)
    printf("%c", buf[i]);
  printf("\x1b[1;31m%c\x1b[0m", buf[diff - move]);
  for (size_t i = diff - move + 1; i < read; i++)
    printf("%c", buf[i]);

  printf("…\n");
}

int handle_compare(command_compare_t args) {
  int64_t diff = compare(args.file_path1, args.file_path2);

  if (diff == -1) {
    return EXIT_SUCCESS;
  } else if (diff == -2) {
    fprintf(stderr, "Error comparing bytes.\n");
    return EXIT_FAILURE;
  }

  printf("Given files differ at position %" PRId64 ":\n", diff);
  print_diff(args.file_path1, diff);
  print_diff(args.file_path2, diff);

  return EXIT_SUCCESS;
}

#undef COMPARE_BUFFER_SIZE
