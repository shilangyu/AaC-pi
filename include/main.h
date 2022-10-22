#pragma once

// used for unrecoverable errors (panics)
#define ERR(source)                                                \
  (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), \
   exit(EXIT_FAILURE))

// panics if a function fails (returns a non-zero integer)
#define CHECK(source)                \
  do {                               \
    if ((source) != 0) ERR(#source); \
  } while (0)
