/** std */
#include <thread>
#include <iostream>
#include <map>
#include <string>
#include <vector>
/** unix */
#include <cstring> // memset
#include <unistd.h> // pid
#include <signal.h> // signal
#include <sys/shm.h> // shared mem (UNIX)
#include <sys/ipc.h> // interprocess comms (UNIX)
#include <sys/types.h> // key (needed for shared mem mapping) (UNIX)
/** concord */
#include <concord/Node.h>

/** mapping */
// template
struct thread {
  pid_t pid;
  struct shm {
    key_t key;
    size_t size;
  };
  *Node node;
};
// actual mapping
std::map<key_t, thread> threads;

/** Exposed Functions */
void Spawn(char kChar);
void Send(pid_t pid, int sig);
void Stop(pid_t pid);

/** Lower-Level (DO NOT CALL) */
void Worker(key_t key);
static void Handle(int sig, siginfo_t *siginfo, void *context);
