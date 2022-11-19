#include "generate.h"
#include "chudnovsky.h"
#include "main.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Quadratic convergence, but expensive
double gauss_legendre() {
  double a = 1.0;
  double b = 1.0 / sqrt(2);
  double t = 1.0 / 4.0;
  double p = 1.0;

  for (size_t i = 0; i < 3; i++) {
    double ak = (a + b) / 2.0;

    b = sqrt(a * b);
    t = t - p * (a - ak) * (a - ak);
    p = 2 * p;
    a = ak;
  }

  return (a + b) * (a + b) / (4 * t);
}

int handle_generate(command_generate_t args) {
  FILE *file = fopen(args.pi_file_path, "w");
  if (file == NULL)
    ERR(args.pi_file_path);

  chudnovsky(file, args.n_digits);

  CHECK(fclose(file));

  return EXIT_SUCCESS;
}
