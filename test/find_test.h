#include "find.h"
#include "io_shim.h"
#include "main.h"
#include "mmap.h"
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>

// not cleaned up
uint8_t *_reaf_file(const char *file, size_t *data_len) {
  int fd = open(file, O_RDONLY);
  if (fd == -1) ERR(file);

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

  *data_len = stats.st_size;

  return data;
}

void find_test(void) {
  size_t size10, size100;
  uint8_t *data10  = _reaf_file("test/samples/pi_10_good.txt", &size10);
  uint8_t *data100 = _reaf_file("test/samples/pi_100_good.txt", &size100);

  assert(kmp(data10, size10, "11111") == -1);
  assert(kmp(data100, size100, "11111") == -1);

  assert(kmp(data10, size10, "92") == 6);
  assert(kmp(data10, size10, "3.") == 0);
  assert(kmp(data10, size10, "5") == 5);
  assert(kmp(data10, size10, "65") == 8);

  assert(kmp(data100, size100, "92") == 6);
  assert(kmp(data100, size100, "3.") == 0);
  assert(kmp(data100, size100, "5") == 5);
  assert(kmp(data100, size100, "65") == 8);
  assert(kmp(data100, size100, "9445923") == 59);

  CHECK(munmap(data10, size10));
  CHECK(munmap(data100, size100));
}
