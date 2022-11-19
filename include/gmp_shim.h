// Shim for GMP to work on windows by compiling from source MPIR
// (https://github.com/BrianGladman/mpir) and embedding it in ./mpir_windows

#ifdef _WIN32
#include "mpir_windows/gmp.h"
#else
#include <gmp.h>
#endif
