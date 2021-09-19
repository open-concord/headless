 /**
* Just thread locking and unlocking
* It's recommended that you write the thread # out to a file,
* otherwise you risk memory leaks
*
* Calling Spawn(key_t, nodeCfg) will create a headless node
* Calling Stop(pid_t) will FORCEFULLY stop the headless node with # pid
* Calling Send(pid_t, int) will send the signal int (Should be used to close Nodes)
*/

#include "hdls.hpp"

// CONFIG //
unsigned int threadNum; // number of concerrent threads to run
size_t threadAlloc; // memory to alloc per-thread [bytes]
// [END] CONFIG //

void Spawn(
  /** ledger config */
  int lid,
  size_t lsize,
  /** node config */
  int nqueue,
  unsigned short int nport,
  std::map<std::string, Tree> ncm,
  boost::function<void (std::unordered_set<std::string>)> nblocks_cb
) {
  key_t lkey = ftok(".", lid);
  std::thread{
    &Worker,
    /** ledger config */
    std::ref(lkey), std::ref(lsize),
    /** node config */
    std::ref(nqueue),
    std::ref(nport),
    std::ref(ncm),
    std::ref(nblocks_cb)
  }.detach();
  // exit(0)
};

void Send(pid_t pid, int sig) {
  kill(pid, sig);
};

void Stop(pid_t pid) {
  kill(pid, SIGQUIT);
};

void Worker(
  /** ledger config */
  std::reference_wrapper<key_t> lkey,
  std::reference_wrapper<size_t> lsize,
  /** node config */
  std::reference_wrapper<int> nqueue,
  std::reference_wrapper<unsigned short int> nport,
  std::reference_wrapper<std::map<std::string, Tree>> ncm,
  std::reference_wrapper<boost::function<void (std::unordered_set<std::string>)>> nblocks_cb
) {
  // binding sigaction
  struct sigaction act;
  std::memset(&act, '\0', sizeof(act));
  act.sa_sigaction = Handle;
  act.sa_flags = SA_SIGINFO; // ask for siginfo

  // establish shm<pid>
  key_t pk = ftok(".", getpid());
  int oid = shmget(pk, threadAlloc, IPC_CREAT);

  // log to shm<key>
  int lid = shmget(lkey, lsize, 0 /**shm (should) already exist*/);
  std::vector<unsigned int> *lptr;
  shmat(lid, lptr, 0);
  lptr->push_back(getpid());
  // detaching from shm
  shmdt(lptr);

  // finally just running a Node instance
  thread *optr;
  shmat(oid, optr, 0);

  // it's ugly, but whatever
  (*optr).node(
    nqueue.get(),
    nport.get(),
    ncm.get(),
    nblocks_cb.get()
  );
  // open Node + start handling
  (*optr).node.start();
  (*optr).node.begin_next();
};

static void Handle(int sig, siginfo_t *siginfo, void *context) {
  // pulling the data
  key_t pk = ftok(".", getpid());
  int shmid = shmget(pk, threadAlloc, 0);
  // get the thread's shared mem
  thread *self;
  shmat(shmid, self, 0);

  switch (sig) {
    case 10: // 10 is just a placeholder, but it's our graceful kill function for now
      (*self).node.stop();
      break;
    default: // missing signal????
      std::cout << "Non-Handled Signal Recieved : " << sig << "\n";
      break;
  };
  // detach from shared memory
  shmdt(self);
};
