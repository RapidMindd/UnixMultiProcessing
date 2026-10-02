#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "utils.h"

const char* SHARED_MEMORY_NAME = "/shm-temp";

int main()
{
  std::string data = {};
  auto& stream = std::getline(std::cin, data);
  if (!stream) {
    return 1;
  }
  int pipes[2] = {};
  int err = pipe(pipes);
  int rd = pipes[0], wr = pipes[1];
  pid_t pid = fork();
  if (pid == 0) {
    close(wr);
    dup2(rd, STDIN_FILENO);
    close(rd);
    execl("child", "child", NULL);
  }
  close(rd);
  auto fl = O_RDWR | O_CREAT | O_TRUNC;
  auto mode = S_IRUSR | S_IWUSR;
  int shared = shm_open(SHARED_MEMORY_NAME, fl, mode);
  ftruncate(shared, data.size());
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_SHARED;
  auto ptr = (char*)mmap(NULL, data.size(), prot, flags, shared, 0);
  memcpy(ptr, data.data(), data.size());
  munmap(ptr, data.size());
  char p[256];
  int len = sprintf(p, "%zu", data.size());
  pass(err, wr, p, len);
  close(wr);
  close(shared);
  waitpid(pid, 0, 0);
  shm_unlink(SHARED_MEMORY_NAME);
}
