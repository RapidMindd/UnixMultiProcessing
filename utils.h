#ifndef UNIXMULTIPROCESSING_UTILS_H
#define UNIXMULTIPROCESSING_UTILS_H

#include <cstddef>
#include <sys/_types/_ssize_t.h>

size_t pass(int& err, int wr, const char* data, size_t k);
size_t get(int& err, int rd, char* data, size_t k);

#endif
