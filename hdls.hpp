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

/** Exposed Functions */
void Spawn(char kChar);
void Send(pid_t pid, int sig);
void Stop(pid_t pid);

/** Configurable Signal Mapping (Incomplete) */
struct dataExt { // data extension
  int shmid;
  size_t size;
};

struct threadExt {
  dataExt self; // information containing the PID of the thread
  std::map<int, void(*handle)(Node)> sigHndl; // signalInt mapped to signal handle
};

/** You should define the sigExt in your implementation */
std::map<int, sigExt> threadMap;

/** END Configurable Signal Mapping (Incomplete) */

/** Lower-Level (DO NOT CALL) */
void Worker(key_t key);
static void Handle(int sig, siginfo_t *siginfo, void *context);
