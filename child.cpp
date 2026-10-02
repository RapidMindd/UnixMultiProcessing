#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "utils.h"

const char* SHARED_MEMORY_NAME = "/shm-temp";

int main()
{
  int num = 0;
  if (scanf("%d", &num) != 1) {
    return 1;
  }
  auto fl = O_RDONLY;
  auto mode = S_IRUSR | S_IWUSR;
  int shared = shm_open(SHARED_MEMORY_NAME, fl, mode);
  if (shared == -1) {
    std::cerr << errno << '\n';
    shm_unlink(SHARED_MEMORY_NAME);
    return 1;
  }
  int prot = PROT_READ, flags = MAP_SHARED;
  auto ptr = (char*)mmap(NULL, num + 1, prot, flags, shared, 0);
  std::string data(ptr, num);
  std::cout << data << '\n';
  munmap(ptr, num);
  close(shared);
}
