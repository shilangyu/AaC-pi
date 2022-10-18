#include "args.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdint.h>
#define COMPARE_BUFFER_SIZE 1024 * 1024

void compare_zero_rest(char *buff, size_t read) {
  int read_delta = COMPARE_BUFFER_SIZE - read;
  if (read_delta == 0)
    return;

  printf("Zeroing %i trailing bytes...\n", read_delta);
  memset(buff + COMPARE_BUFFER_SIZE - read_delta, 0, read_delta);
}

// <-1: error:
//      -2: comparison error
//  -1: equals
// >-1: first diff index
int64_t compare(const char *file_name_1, const char *file_name_2) {
  int64_t result = -1;

  char buff_1[COMPARE_BUFFER_SIZE];
  char buff_2[COMPARE_BUFFER_SIZE];

  int64_t *buff_1_64 = (int64_t *) buff_1;
  int64_t *buff_2_64 = (int64_t *) buff_2;

  FILE *file_1 = fopen(file_name_1, "r");
  FILE *file_2 = fopen(file_name_2, "r");

  for (int64_t iter = 0; result == -1; iter++) {
    size_t read_1 = fread(buff_1, 1, COMPARE_BUFFER_SIZE, file_1);
    size_t read_2 = fread(buff_2, 1, COMPARE_BUFFER_SIZE, file_2);

    compare_zero_rest(buff_1, read_1);
    compare_zero_rest(buff_2, read_2);

    for (int64_t i = 0; i < COMPARE_BUFFER_SIZE / sizeof(int64_t); i++) {
      if (buff_1_64[i] == buff_2_64[i])
        continue;

      // for error checking
      result = -2;

      for (int64_t j = i * sizeof(int64_t); j < (i + 1) * sizeof(int64_t); j++)
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

  fclose(file_1);
  fclose(file_2);

  return result;
}

int main(const int argc, const char **argv) {
  int64_t result = compare("a.txt", "b.txt");
  printf("\nCompare result: %li\n\n", result);

  args_t args = parse_args(argv, argc);

  // prints first 800 digits of pi,
  // source: https://cs.uwaterloo.ca/~alopez-o/math-faq/mathtext/node12.html
  int a = 10000, b = 0, c = 2800, d, e, f[2801], g;
  for (; b - c;)
    f[b++] = a / 5;
  for (; d = 0, (g = c * 2); c -= 14, printf("%.4d", e + d / a), e = d % a)
    for (b = c; d += f[b] * a, f[b] = d % --g, d /= g--, --b; d *= b)
      ;
}
