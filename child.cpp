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
  scanf("%d", &num);
  auto fl = O_RDONLY;
  auto mode = S_IRUSR | S_IWUSR;
  int shared = shm_open(SHARED_MEMORY_NAME, fl, mode);
  int prot = PROT_READ, flags = MAP_SHARED;
  auto ptr = (char*)mmap(NULL, num, prot, flags, shared, 0);
  std::string data(ptr, num);
  std::cout << data << '\n';
  munmap(ptr, num);
  close(shared);
}
