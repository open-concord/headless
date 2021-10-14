#include "../inc/hdls.hpp"
#include <fstream>


int Spawn(config cfg) {
  pid_t pid;
  pid = fork(); // [u2on] need to make this multi-platform safe
  if (pid > 0) {
    return pid; // parent
  }
  Worker(cfg);
  return 0; // child
}

void Send(pid_t pid, int sig) {
  _sigSend(pid, sig);
}

void Stop(pid_t pid) {
  _sigSend(pid, 9);
}

void Worker(config cfg) {
  _sigRegister(); // binding to SIGUSR1
  Filler *wptr = shmAttach(cfg); // attach worker shm to process
  wptr->Start(); // finally just running a Node instance
}

void Handle(int sig) {
  Filler *wptr = shmAttach(std::nullopt); // attach worker shm to process
  std::cout << "From within sig handler" << std::endl;
  wptr->Stop();

  shmDestory(std::to_string(_pid()).c_str()); // queue kernal to remove shm

  shmDetach(wptr); // detach from shm
  exit(sig);
}
