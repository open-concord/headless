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

// CONFIG //
size_t threadNum; // number of concerrent threads to run
size_t threadAlloc; // memory to alloc per-thread
// [END] CONFIG //

void Spawn(char kChar) {
  key_t key = ftok(".", kChar);
  std::thread(&Worker, key).detach();
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

};

static void Handle(int sig, siginfo_t *siginfo, void *context) {
  // pulling the data
  std::string mem = std::to_string(getpid());
  int shmid = shmget(mem, threadAlloc);
  // get content
  thread *content = (auto*) shmat(shmid, NULL, 0);
  // YOU SHOULD HANDLE HERE

  // [END] YOU SHOULD HANDLE HERE
  // detach from shared memory
  shmdt(content);
};
