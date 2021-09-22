#include "hdls.hpp"

void Spawn(
  /** ledger config */
  int lid,
  size_t lsize,
  /** thread config (contains node config) */
  config cfg
) {
  key_t lkey = ftok(".", lid);
  std::thread{
    &Worker,
    /** ledger config */
    lkey, lsize,
    /** <filler> node config */
    cfg
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
  key_t lkey,
  size_t lsize,
  /** filler config (contains node config) */
  config cfg
) {
  // binding sigaction
  struct sigaction act;
  std::memset(&act, '\0', sizeof(act));
  act.sa_sigaction = &Handle;
  act.sa_flags = SA_SIGINFO; // ask for siginfo

  // establish shm<pid>
  key_t pk = ftok(".", getpid());
  int oid = shmget(pk, 2000 /* mem per thread */, IPC_CREAT);

  // log to shm<key>
  int lid = shmget(lkey, lsize, 0 /**shm (should) already exist*/);
  auto lptr = (std::vector<unsigned int>* ) shmat(lid, NULL, 0);
  lptr->push_back(getpid());

  // detaching from shm
  shmdt(lptr);

  // finally just running a Node instance
  // NOTE [u2on]: optr is an implicitly declared Filler, but I've found no way to give args (which would fix the seg fault)
  auto optr = (Filler*)/**(cfg.fint)*/ shmat(oid, NULL, 0);

  // call filler method
  optr->Start();
};

static void Handle(int sig, siginfo_t *siginfo, void *context) {
  // pulling the data
  key_t pk = ftok(".", getpid());
  int shmid = shmget(pk, 2000 /* mem per thread */, 0);
  // get the thread's shared mem
  auto optr = (Filler*) shmat(shmid, NULL, 0);

  switch (sig) {
    case 10: // 10 is just a placeholder, but it's our graceful kill function for now
      optr->Stop();
      shmctl(shmid, IPC_RMID, NULL); // delete mem
      break;
    default: // missing signal????
      std::cout << "Non-Handled Signal Recieved : " << sig << "\n";
      break;
  };
  // detach from shared memory
  shmdt((void *) optr);
};
