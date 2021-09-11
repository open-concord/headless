#include <thread>
#include <iostream>
#include <cstring> // memset
#include <unistd.h> // pid
#include <signal.h> // signal
#include <sys/shm.h> // shared mem (UNIX)
#include <sys/ipc.h> // interprocess comms (UNIX)
#include <sys/types.h> // key (needed for shared mem mapping) (UNIX)


void Spawn(char kChar);
void Send(pid_t pid, int sig);
void Stop(pid_t pid);

class Worker {
private:
  key_t shared;
  pid_t pid;
public:
  Worker(key_t key);
  static void Handle(int sig, siginfo_t *siginfo, void *context);
};
