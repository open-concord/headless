#include "../src/hdls.hpp"

#include <vector>
#include <iostream>
#include <string>

#include <stdlib.h> // strtol
// g++ src/hdls.hpp src/hdls.cpp src/filler.cpp test/example.cpp -o test.out -lpthread
// ^ from project root

std::vector<unsigned int> ledger;

// dummy config
config cfg = {6};

int main (int argc, char** argv) { // 0 to kill, 1 to spawn
  std::cout << "CMD: " << argv[1] << "\n";
  char *temp;
  long cmd = strtol(argv[1], &temp, 10);
  switch (cmd) {
    case 0:
    { // explicit wrapping
        int lid = shmget(1337, 288, 0);
        std::vector<unsigned int> *lptr;
        shmat(lid, lptr, 0);
        for (int i=0; i<lptr->size();i++) {
          Send(lptr->at(i), 10);
      };
      break;
    }
    case 1:
    { // explicit wrapping
      for (int i=0; i<3; i++) {
        Spawn(
          /** ledger */
          1337, 288, /** 32 bytes per PID, 3 nodes = 288 bytes*/
          /** node */
          cfg
        );
      };
      break;
    }
  }
  return 0;
}
