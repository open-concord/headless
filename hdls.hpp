#include <thread>
#include <iostream>
#include <map>
#include <vector>
#include <cstring> // memset
#include <unistd.h> // pid
#include <signal.h> // signal
#include <sys/shm.h> // shared mem (UNIX)
#include <sys/ipc.h> // interprocess comms (UNIX)
#include <sys/types.h> // key (needed for shared mem mapping) (UNIX)

/** Exposed Functions */
void Spawn(char kChar);
void Send(pid_t pid, int sig);
void Stop(pid_t pid);

/** Configurable Signal Mapping */
struct dataExt { // data extension
  int shmid;
  size_t size;
};

struct sigExt { // signal extension
  dataExt self; // self info
  std::vector<dataExt> moduleData; // module specific data
  void(*handle)(Node); // handle func
};

/** You should define this in your implementation */
std::map<int, sigExt> sigMap;

/** Lower-Level (DO NOT CALL) */
void Worker(key_t key);
static void Handle(int sig, siginfo_t *siginfo, void *context);
