#include "chudnovsky.h"
#include <gmp.h>
#include <math.h>
#include <stdlib.h>

void chudnovsky_bs(uint64_t a, uint64_t b,
                   mpz_t a_factor,
                   mpz_t *p_stack,
                   mpz_t *q_stack,
                   mpz_t *t_stack,
                   size_t stack_pointer) {
  // Adjacent terms in binary splitting X(a, a-1)
  if (b - a == 1) {
    if (a == 0) {
      // Set initial values
      mpz_set_ui(p_stack[stack_pointer], 1);
      mpz_set_ui(q_stack[stack_pointer], 1);
    } else {
      // We have to split P computation into steps because the polynomial would overflow uint64
      mpz_set_ui(p_stack[stack_pointer], 6 * a - 5);
      mpz_mul_ui(p_stack[stack_pointer], p_stack[stack_pointer], 2 * a - 1);
      mpz_mul_ui(p_stack[stack_pointer], p_stack[stack_pointer], 6 * a - 1);
      mpz_mul_ui(q_stack[stack_pointer], a_factor, a);
      mpz_mul_ui(q_stack[stack_pointer], q_stack[stack_pointer], a);
      mpz_mul_ui(q_stack[stack_pointer], q_stack[stack_pointer], a);
    }

    mpz_set_ui(t_stack[stack_pointer], a);
    mpz_mul_ui(t_stack[stack_pointer], t_stack[stack_pointer], 545140134);
    mpz_add_ui(t_stack[stack_pointer], t_stack[stack_pointer], 13591409);
    mpz_mul(t_stack[stack_pointer], t_stack[stack_pointer], p_stack[stack_pointer]);

    // Negate for odd a
    if (a & 1) {
      mpz_neg(t_stack[stack_pointer], t_stack[stack_pointer]);
    }
  } else {
    // Split
    uint64_t m = (a + b) / 2;
    chudnovsky_bs(a, m, a_factor, p_stack, q_stack, t_stack, stack_pointer);
    chudnovsky_bs(m, b, a_factor, p_stack, q_stack, t_stack, stack_pointer + 1);

    // At this point, stack_pointer is X(a, m) and stack_pointer+1 is X(m, b)
    // Order of operations here is important, it was carefully chosen to reuse
    // Registers without needing for a temporary variable. The following is calculated:
    // Q(a, b) = Q(a, m) * Q(m, b)
    // T(a, b) = T(a, m) * Q(m, b) + P(a, m) * T(m, b)
    // P(a, b) = P(a, m) * P(m, b)

    mpz_mul(q_stack[stack_pointer], q_stack[stack_pointer], q_stack[stack_pointer + 1]);

    mpz_mul(t_stack[stack_pointer], t_stack[stack_pointer], q_stack[stack_pointer + 1]);
    // For T, we reuse Q(m, b) register, we won't need it anymore
    mpz_mul(q_stack[stack_pointer + 1], p_stack[stack_pointer], t_stack[stack_pointer + 1]);
    mpz_add(t_stack[stack_pointer], t_stack[stack_pointer], q_stack[stack_pointer + 1]);

    mpz_mul(p_stack[stack_pointer], p_stack[stack_pointer], p_stack[stack_pointer + 1]);
  }
}

// Linear convergence, binary splitting
void chudnovsky(FILE *output_file, uint64_t n_digits) {
  // As per theorem 10.13 from Chudnovsky formula proof: https://arxiv.org/abs/1809.00533
  double digits_per_iter = log10(53360.0 * 53360.0 * 53360.0);
  // The amount of iterations needed to obtain n_digits.
  // +1 to make sure all digits are calculated.
  uint64_t iter_count = n_digits / digits_per_iter + 1;

  // The size of the stack needed for recursive binary partitioning.
  size_t stack_size = ceil(log2(iter_count)) + 1;

  // Stack for the binary partitioning terms P Q and T
  mpz_t *p_stack = malloc(sizeof(mpz_t) * stack_size);
  mpz_t *q_stack = malloc(sizeof(mpz_t) * stack_size);
  mpz_t *t_stack = malloc(sizeof(mpz_t) * stack_size);
  for (size_t i = 0; i < stack_size; i++) {
    mpz_init(p_stack[i]);
    mpz_init(q_stack[i]);
    mpz_init(t_stack[i]);
  }

  // Constant factor term for consecutive A sequence
  mpz_t a_factor;
  mpz_init(a_factor);
  mpz_set_ui(a_factor, 640320);
  mpz_mul_ui(a_factor, a_factor, 640320);
  mpz_mul_ui(a_factor, a_factor, 640320);
  mpz_div_ui(a_factor, a_factor, 24);

  // Run binary splitting at the highest level
  chudnovsky_bs(0, iter_count, a_factor, p_stack, q_stack, t_stack, 0);

  // Calculate the final value of π
  mpz_t pi;
  mpz_init(pi);
  // Scale pi to integer
  mpz_set_ui(pi, 10);
  mpz_pow_ui(pi, pi, 2 * n_digits);
  mpz_mul_ui(pi, pi, 10005);
  mpz_sqrt(pi, pi);
  mpz_mul_ui(pi, pi, 426880);
  mpz_mul(pi, pi, q_stack[0]);
  mpz_cdiv_q(pi, pi, t_stack[0]);

  // Output the calculated π to a file
  mpz_out_str(output_file, 10, pi);

  // cleanup resources
  mpz_clear(a_factor);
  mpz_clear(pi);
  for (size_t i = 0; i < stack_size; i++) {
    mpz_clear(p_stack[i]);
    mpz_clear(q_stack[i]);
    mpz_clear(t_stack[i]);
  }
  free(p_stack);
  free(q_stack);
  free(t_stack);
}
