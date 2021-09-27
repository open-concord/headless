#include "hdls.hpp"

int Spawn(
  /** thread config (contains node config) */
  config cfg
) {
  pid_t pid;
  pid = fork();
  if (pid > 0) {
    return pid; // parent
  }
  Worker(cfg);
  return 0; // child
};

void Send(pid_t pid, int sig) {
  kill(pid, sig);
};

void Stop(pid_t pid) {
  kill(pid, SIGQUIT);
};

/** convience method to reduce clutter */
int _keying(const char* k, std::size_t s) {
  // create shm, set size
  int km = shm_open(k, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if ((km || ftruncate(km, s)) < 0) {
    return -1;
  };
  return km;
};

void Worker(
  /** filler config (contains node config) */
  config cfg
) {

  // binding sigaction
  struct sigaction act;
  std::memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &Handle;
  act.sa_flags = SA_SIGINFO; // ask for siginfo

  // map worker shm to process [wptr]
  Filler wf(3);
  Filler* wptr = &wf;
  wptr = static_cast<Filler*>(mmap(
    NULL,
    alloc,
    PROT_READ | PROT_WRITE,
    MAP_SHARED,
    _keying(std::to_string(getpid()).c_str(), alloc),
    0
  ));
  try {
    if (wptr == MAP_FAILED) {
      // error
    };
  } catch (...) {/** ISO C++ forbids comparison between pointer and integer */}

  // finally just running a Node instance
  wptr->Start();
};

static void Handle(int sig, siginfo_t *siginfo, void *context) {
  // map worker shm to process [wptr]
  Filler *wptr;
  wptr = static_cast<Filler*>(mmap(
    NULL,
    alloc,
    PROT_READ | PROT_WRITE,
    MAP_SHARED,
    _keying(std::to_string(getpid()).c_str(), alloc),
    0
  ));
  try {
    if (wptr == MAP_FAILED) {
      // error
    };
  } catch (...) {/** ISO C++ forbids comparison between pointer and integer */}
  
  switch (sig) {
    case 10: // 10 is just a placeholder, but it's our graceful kill function for now
      wptr->Stop();
      shm_unlink(std::to_string(getpid()).c_str()); // remove shm
      break;
    default: // missing signal????
      break;
  };
  // detach from shm
  if (munmap(wptr, alloc) < 0) {
    // error
    exit(1);
  };
  exit(0);
};
