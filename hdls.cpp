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

void Spawn(char kChar, nodeCfg cfg) {
  key_t key = ftok(".", kChar);
  std::thread(&Worker, key, cfg).detach();
  exit(0);
};

void Send(pid_t pid, int sig) {
  kill(pid, sig);
};

void Stop(pid_t pid) {
  kill(pid, SIGQUIT);
};

void Worker(key_t key, nodeCfg cfg) {
  // set pid
  pid_t pid = getpid();

  // binding sigaction
  struct sigaction act;
  std::memset(&act, '\0', sizeof(act));
  act.sa_sigaction = Handle;
  act.sa_flags = SA_SIGINFO; // ask for siginfo

  // establish shm<pid>
  int oid = shmget(std::to_string(pid), threadAlloc, IPC_CREAT);

  // log to shm<key>
  int lid = shmget(
    key,
    // each stringified PID is 32 bytes
    (32*threadNum)
  );
  ledger *lptr = (ledger *) shmat(lid, NULL, 0);
  for (var i=0; i<threadNum; i++) {
    if(*lptr[i] == "00000") {
      *lptr[i] = std::to_string(pid);
      return;
    };
  };
  // detaching from shm
  shmdt(lptr);

  // finally just running a Node instance
  thread *optr = (thread *) shmat(oid, NULL, 0);

  // it's ugly, but whatever
  *optr.node = new Node(
    cfg.queue,
    cfg.port,
    cfg.cm,
    &cfg.blocks_cb
  );
  // open Node + start handling
  *optr.node::start();
  *optr.node::begin_next();
};

static void Handle(int sig, siginfo_t *siginfo, void *context) {
  // pulling the data
  std::string mem = std::to_string(getpid());
  int shmid = shmget(mem, threadAlloc);
  // get the thread's shared mem
  thread *self = (auto*) shmat(shmid, NULL, 0);
  switch (sig) {
    case 10: // 10 is just a placeholder, but it's our graceful kill function for now
      *self.node::close();
      break;
    default: // missing signal????
      std::cout << "Non-Handled Signal Recieved : " << sig << "\n";
      break;
  };
  // detach from shared memory
  shmdt(self);
};
