#include "compare.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

#define COMPARE_BUFFER_SIZE 1024 * 1024

void compare_zero_rest(uint8_t *buff, size_t read) {
  int read_delta = COMPARE_BUFFER_SIZE - read;
  if (read_delta == 0)
    return;

  memset(buff + COMPARE_BUFFER_SIZE - read_delta, 0, read_delta);
}

// <-1: error:
//      -2: comparison error
//  -1: equals
// >-1: first diff index
int64_t compare(const char *file_name1, const char *file_name2) {
  int64_t result = -1;

  uint8_t buff_1[COMPARE_BUFFER_SIZE];
  uint8_t buff_2[COMPARE_BUFFER_SIZE];

  uint64_t *buff_1_64 = (uint64_t *)buff_1;
  uint64_t *buff_2_64 = (uint64_t *)buff_2;

  FILE *file_1 = fopen(file_name1, "r");
  if (file_1 == NULL) ERR(file_name1);
  FILE *file_2 = fopen(file_name2, "r");
  if (file_2 == NULL) ERR(file_name2);

  for (uint64_t iter = 0; result == -1; iter++) {
    size_t read_1 = fread(buff_1, 1, COMPARE_BUFFER_SIZE, file_1);
    size_t read_2 = fread(buff_2, 1, COMPARE_BUFFER_SIZE, file_2);

    compare_zero_rest(buff_1, read_1);
    compare_zero_rest(buff_2, read_2);

    for (uint64_t i = 0; i < COMPARE_BUFFER_SIZE / sizeof(int64_t); i++) {
      if (buff_1_64[i] == buff_2_64[i])
        continue;

      // for error checking
      result = -2;

      for (uint64_t j = i * sizeof(uint64_t); j < (i + 1) * sizeof(uint64_t); j++)
        if (buff_1[j] != buff_2[j]) {
          result = j + iter * COMPARE_BUFFER_SIZE;
          break;
        }

      break;
    }

    // break if EOF
    if (read_1 < COMPARE_BUFFER_SIZE || read_2 < COMPARE_BUFFER_SIZE)
      break;
  }

  CHECK(fclose(file_1));
  CHECK(fclose(file_2));

  return result;
}

int handle_compare(command_compare_t args) {
  return compare(args.file_path1, args.file_path2) != -1;
}

#undef COMPARE_BUFFER_SIZE
