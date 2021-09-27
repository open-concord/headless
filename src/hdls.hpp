/** std */
#include <thread>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>
#include <memory>
/** unix */
#include <cstring> // memset
#include <unistd.h> // pid
#include <signal.h> // signal
#include <sys/mman.h> // shared mem (UNIX)
#include <sys/stat.h> // mode const (UNIX)
#include <fcntl.h> // O_ const (UNIX)
/** concord */
//#include <concord/node.h>
//#include <concord/tree.h>

/** filler class, used for testing (This would be a Concord/Node) */
class Filler {
private:
  int ft;
public:
  Filler(int fint);
  void Start();
  void Stop();
};

// config for filler object
struct config {
  int fint;
};

// config for individual node memory allocation
#define alloc 4096 // 4kb for testing

/** Exposed Functions */
void Spawn(
  /** pid for the caller*/
  std::vector<int> ledger,
  /** thread config (contains node config) */
  config cfg
);

void Send(pid_t pid, int sig);
void Stop(pid_t pid);

/** Lower-Level */
void Worker(
  /** pid for the caller*/
  std::vector<int>& ledger,
  /** thread config (contains node config) */
  config cfg
);
static void Handle(int sig, siginfo_t *siginfo, void *context);
