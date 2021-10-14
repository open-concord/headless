// Linux specific implementation of sysapi calls
#include "../../inc/hdls.hpp"
// platform specific libs
#include <cstring> // memset
#include <unistd.h> // pid
#include <signal.h> // signal
#include <sys/mman.h> // shared mem
#include <sys/stat.h> // mode const
#include <fcntl.h> // O_ const

void _sigRegister() {
  signal(SIGUSR1, &Handle);
  /*
  struct sigaction act = {0};
  act.sa_handler = &Handle;

  if (sigaction(SIGUSR1, &act, NULL)<0) {
    std::cout << "ERROR IN SIGACT" << std::endl;
    exit(1);
  }
  */
}

void _sigSend(int pid, int sig) {
  kill(pid, sig);
}

int _keying(const char* k, std::size_t s) {
  // create shm, set size
  int km = shm_open(k, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (km<0 || ftruncate(km, s)<0) {
    return -1;
  };
  return km;
}

int _pid() {
  return getpid();
}

Filler* shmAttach(std::optional<config> cfg) {
  Filler* wp;
  if (cfg.has_value()) {wp = new Filler(cfg->fint);}
  wp = static_cast<Filler*>(mmap(
    NULL,
    alloc,
    PROT_READ | PROT_WRITE,
    MAP_SHARED,
    _keying(std::to_string(getpid()).c_str(), alloc),
    0
  ));
  return wp;
}

int shmDetach(Filler* ptr) {
  if (munmap(ptr, alloc) < 0) return -1;
  return 0;
}

void shmDestory(const char* target) {
  shm_unlink(target);
}
