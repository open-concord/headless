/** std */
#include <thread>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
/** unix */
#include <cstring> // memset
#include <unistd.h> // pid
#include <signal.h> // signal
#include <sys/shm.h> // shared mem (UNIX)
#include <sys/ipc.h> // interprocess comms (UNIX)
#include <sys/types.h> // key (needed for shared mem mapping) (UNIX)
/** concord */
#include <concord/Node.h>
#include <concord/Tree.h>

/** node config */
struct nodeCfg {
  int queue;
  unsigned short int port;
  std::map<std::string, Tree> cm;
  (void*)(blocks_cb)(std::unordered_set<std::string>);
}

/** mapping */
// template [per-thread] (right now pretty barren)
struct thread {
  *Node node;
};
// template [thread ledger]
//std::array<std::string, threadNum> ledger;
//ledger.fill("00000")


/** Exposed Functions */
void Spawn(char kChar);
void Send(pid_t pid, int sig);
void Stop(pid_t pid);

/** Lower-Level (DO NOT CALL) */
void Worker(key_t key, nodeCfg cfg);
static void Handle(int sig, siginfo_t *siginfo, void *context);
