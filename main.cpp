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
  if (err) {
    std::cerr << errno << '\n';
    return 1;
  }
  int rd = pipes[0], wr = pipes[1];
  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << errno << '\n';
    return 1;
  }
  if (pid == 0) {
    close(wr);
    err = dup2(rd, STDIN_FILENO);
    if (err == -1) {
      std::cerr << errno << '\n';
      _exit(1);
    }
    close(rd);
    execl("child", "child", NULL);
    std::cerr << errno << '\n';
    _exit(1);
  }
  close(rd);
  auto fl = O_RDWR | O_CREAT | O_TRUNC;
  auto mode = S_IRUSR | S_IWUSR;
  int shared = shm_open(SHARED_MEMORY_NAME, fl, mode);
  if (shared == -1) {
    std::cerr << errno << '\n';
    return 1;
  }
  err = ftruncate(shared, data.size());
  if (err == -1) {
    std::cerr << errno << '\n';
    shm_unlink(SHARED_MEMORY_NAME);
    return 1;
  }
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_SHARED;
  auto ptr = (char*)mmap(NULL, data.size() + 1, prot, flags, shared, 0);
  memcpy(ptr, data.data(), data.size());
  munmap(ptr, data.size());
  char p[256];
  int len = sprintf(p, "%d", data.size());
  pass(err, wr, p, len);
  if (err <= 0) {
    shm_unlink(SHARED_MEMORY_NAME);
    return 1;
  }
  close(wr);
  close(shared);
  err = waitpid(pid, 0, 0);
  if (err != pid) {
    std::cerr << errno << '\n';
    shm_unlink(SHARED_MEMORY_NAME);
    return 1;
  }
  shm_unlink(SHARED_MEMORY_NAME);
}
