/**
* Just thread locking and unlocking
* It's recommended that you write the thread # out to a file,
* otherwise you risk memory leaks
*
* Calling Spawn(#) will create a headless node with the shm key'd to #
* Calling Stop(#) will FORCEFULLY stop the headless node with # pid
* Calling Send(#, #') will send process # the signal #' (Should be used to close Nodes)
*/

#include "hdls.hpp"

void Spawn(char kChar) {
  key_t key = ftok(".", kChar);
  std::thread threadObj(new Worker, key);
  threadObj.detach();
};

void Send(pid_t pid, int sig) {
  kill(pid, sig);
};

void Stop(pid_t pid) {
  kill(pid, SIGQUIT);
};

Worker::Worker(key_t key) {
  // set pid
  this->pid = getpid();

  // binding sigaction
  struct sigaction act;
  std::memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &Worker::Handle;
  act.sa_flags = SA_SIGINFO; // ask for siginfo

  // establish shm + store pid in shm
  int shmid = shmget(key, sizeof(&this->pid), IPC_CREAT);
  pid_t *shmptr = (pid_t *) shmat(shmid, NULL, 0);
  *shmptr = this->pid;

  // finally just running a Node instance

};

void Worker::Handle(int sig, siginfo_t *siginfo, void *context) {
  std::cout << "SIGNAL RECIEVED" << "\n";
  std::cout << "GET own PID: " << (long)siginfo->si_pid << "\n";
  std::cout << "GET own UID: " << (long)siginfo->si_uid << "\n";
};
