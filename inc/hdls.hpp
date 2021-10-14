/** std */
#include <thread>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>
#include <memory>
#include <optional>

/** concord */
//#include <concord/node.h>
//#include <concord/tree.h>

/** filler class, used for testing (This would be a Concord/Node) */
class Filler {
private:
  int ft;
  bool done;
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
int Spawn(config cfg);

void Send(pid_t pid, int sig);
void Stop(pid_t pid);

/** Lower-Level */
void Worker(config cfg);
void Handle(int sig);

/** platform specific sysapi stuff */
void _sigRegister();
void _sigSend(int pid, int sig);
int _keying();
int _pid();
Filler* shmAttach(std::optional<config> cfg);
int shmDetach(Filler* ptr);
void shmDestory(const char* target);
