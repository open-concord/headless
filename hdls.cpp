/**
* Just thread locking and unlocking
* It's recommended that you write the thread # out to a file,
* otherwise you risk memory leaks
*
* Calling Spawn(key_t) will create a headless node with the specified shm
* Calling Stop(pid_t) will FORCEFULLY stop the headless node with # pid
* Calling Send(pid_t, int) will send the signal int (Should be used to close Nodes)
*/

#include "hdls.hpp"

void Spawn(char kChar) {
  key_t key = ftok(".", kChar);

  std::thread(&Handle, key).detach();
  exit(0);
};

void Send(pid_t pid, int sig) {
  kill(pid, sig);
};

void Stop(pid_t pid) {
  kill(pid, SIGQUIT);
};

void Worker(key_t key) {
  // set pid
  pid_t pid = getpid();

  // binding sigaction
  struct sigaction act;
  std::memset(&act, '\0', sizeof(act));
  act.sa_sigaction = Handle;
  act.sa_flags = SA_SIGINFO; // ask for siginfo

  // establish shm
  auto threads[key].shm = selfSHM; // shortcut
  int shmid = shmget(key, selfSHM.size, IPC_CREAT);
  // storing PID in shm, so that the calling process call log it
  pid_t *shmptr = (pid_t *) shmat(shmid, NULL, 0);
  *shmptr = pid;

  // document info in mem
  threads[key].pid = pid;

  // finally just running a Node instance

};

static void Handle(int sig, siginfo_t *siginfo, void *context) {
  // pulling the data
  Node *nodeInstance;
  for (auto const& pt : threads) {
    if (pt.pid = getpid()) {
      int shmid = shmget(pt.shm.key, pt.shm.size, IPC_CREAT);
      nodeInstance = (Node*) shmat(shmid,(void*)0,0);
    }
  };
  // YOU SHOULD HANDLE HERE

  // [END] YOU SHOULD HANDLE HERE
  // detach from shared memory
  shmdt(nodeInstance);
};
