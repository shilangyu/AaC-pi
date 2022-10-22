#include "generate.h"
#include <stdio.h>
#include <stdlib.h>

int handle_generate(command_generate_t args) {
  // TODO
  // prints first 800 digits of pi,
  // source: https://cs.uwaterloo.ca/~alopez-o/math-faq/mathtext/node12.html
  int a = 10000, b = 0, c = 2800, d, e, f[2801], g;
  for (; b - c;)
    f[b++] = a / 5;
  for (; d = 0, (g = c * 2); c -= 14, printf("%.4d", e + d / a), e = d % a)
    for (b = c; d += f[b] * a, f[b] = d % --g, d /= g--, --b; d *= b)
      ;

  return EXIT_SUCCESS;
}
