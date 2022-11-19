// Shim for io functions. Windows uses a different header than POSIX systems.

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
