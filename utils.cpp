#include "utils.h"

#include <unistd.h>

size_t pass(int& err, int wr, const char* data, size_t k)
{
  size_t sent = 0;
  while (sent < k) {
    err = write(wr, data + sent, k - sent);
    if (err < 0) {
      break;
    }
    sent += err;
  }
  return sent;
}

size_t get(int& err, int rd, char* data, size_t k)
{
  size_t received = 0;
  while (received < k) {
    err = read(rd, data + received, k - received);
    if (err < 0) {
      break;
    }
    received += err;
  }
  return received;
}
