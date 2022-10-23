#include "find.h"
#include <assert.h>

void find_test(void) {
  assert(kmp("test/samples/pi_10_good.txt", "11111") == -1);
  assert(kmp("test/samples/pi_100_good.txt", "11111") == -1);

  assert(kmp("test/samples/pi_10_good.txt", "92") == 6);
  assert(kmp("test/samples/pi_10_good.txt", "3.") == 0);
  assert(kmp("test/samples/pi_10_good.txt", "5") == 5);
  assert(kmp("test/samples/pi_10_good.txt", "65") == 8);

  assert(kmp("test/samples/pi_100_good.txt", "92") == 6);
  assert(kmp("test/samples/pi_100_good.txt", "3.") == 0);
  assert(kmp("test/samples/pi_100_good.txt", "5") == 5);
  assert(kmp("test/samples/pi_100_good.txt", "65") == 8);
  assert(kmp("test/samples/pi_100_good.txt", "9445923") == 59);
}
