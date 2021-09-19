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
#include <concord/node.h>
#include <concord/tree.h>
// working on moving concord/main away from boost::function to c style function pointers, but have to use this in the interm
#include <boost/function.hpp>

/** mapping */
// template [per-thread] (right now pretty barren)
struct thread {
  Node node;
};

/** Exposed Functions */
void Spawn(
  /** ledger config */
  int lid,
  size_t lsize,
  /** node config */
  std::reference_wrapper<int> nqueue,
  unsigned short int nport,
  std::map<std::string, Tree> ncm,
  boost::function<void (std::unordered_set<std::string>)> nblocks_cb
);

void Send(pid_t pid, int sig);
void Stop(pid_t pid);

/** Lower-Level (DO NOT CALL) */
void Worker(
  /** ledger config */
  std::reference_wrapper<key_t> lkey,
  std::reference_wrapper<size_t> lsize,
  /** node config */
  std::reference_wrapper<int> nqueue,
  std::reference_wrapper<unsigned short int> nport,
  std::reference_wrapper<std::map<std::string, Tree>> ncm,
  std::reference_wrapper<boost::function<void (std::unordered_set<std::string>)>> nblocks_cb
);
static void Handle(int sig, siginfo_t *siginfo, void *context);
