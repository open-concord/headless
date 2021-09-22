/** std */
#include <thread>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>
/** unix */
#include <cstring> // memset
#include <unistd.h> // pid
#include <signal.h> // signal
#include <sys/shm.h> // shared mem (UNIX)
#include <sys/ipc.h> // interprocess comms (UNIX)
#include <sys/types.h> // key (needed for shared mem mapping) (UNIX)
/** concord */
//#include <concord/node.h>
//#include <concord/tree.h>

// config for filler object
struct config {
  int fint;
};

/** filler class, used for testing (This would be a Concord/Node) */
class Filler {
private:
  int ft;
public:
  Filler(int fint);
  void Start();
  void Stop();
};

/** Exposed Functions */
void Spawn(
  /** ledger config */
  int lid,
  size_t lsize,
  /** filler config */
  config cfg
);

void Send(pid_t pid, int sig);
void Stop(pid_t pid);

/** Lower-Level */
void Worker(
  /** ledger config */
  key_t lkey,
  size_t lsize,
  /** filler config */
  config cfg
);
static void Handle(int sig, siginfo_t *siginfo, void *context);
