#include "compare.h"
#include <assert.h>

void compare_test(void) {
  assert(compare("test/samples/pi_10_good.txt", "test/samples/pi_10_good.txt") == -1);
  assert(compare("test/samples/pi_100_good.txt", "test/samples/pi_100_good.txt") == -1);

  assert(compare("test/samples/pi_10_good.txt", "test/samples/pi_10_bad.txt") == 7);
  assert(compare("test/samples/pi_10_bad.txt", "test/samples/pi_10_good.txt") == 7);
  assert(compare("test/samples/pi_100_good.txt", "test/samples/pi_100_bad.txt") == 35);
  assert(compare("test/samples/pi_100_bad.txt", "test/samples/pi_100_good.txt") == 35);

  assert(compare("test/samples/pi_10_good.txt", "test/samples/pi_100_bad.txt") == 10);
  assert(compare("test/samples/pi_10_bad.txt", "test/samples/pi_100_good.txt") == 7);
  assert(compare("test/samples/pi_100_good.txt", "test/samples/pi_10_bad.txt") == 7);
  assert(compare("test/samples/pi_100_bad.txt", "test/samples/pi_10_good.txt") == 10);
}
