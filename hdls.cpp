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

  // establish shm + store pid in shm
  int shmid = shmget(key, sizeof(&pid) /** change this to actual max size */, IPC_CREAT);
  pid_t *shmptr = (pid_t *) shmat(shmid, NULL, 0);
  *shmptr = pid;

  // finally just running a Node instance

};

static void Handle(int sig, siginfo_t *siginfo, void *context) {
  // pulling the data
  int shmid = shmget(sigMap[sig].info.key, sigMap[sig].info.size, IPC_CREAT);
  Node *data = (Node*) shmat(shmid,(void*)0,0);
  // handling
  sigMap[sig].handle(*data);
  // detach from shared memory
  shmdt(data);
};
