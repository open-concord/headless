#include "../src/hdls.hpp"

#include <vector>
#include <iostream>
#include <string>

#include <stdlib.h> // strtol
// g++ src/hdls.hpp src/hdls.cpp src/filler.cpp test/example.cpp -o test.out -lpthread
// ^ from project root

std::vector<int> ledger;

// dummy config
config cfg = {6};

int main (int argc, char** argv) { // 0 to kill, 1 to spawn
  std::cout << "CMD: " << argv[1] << "\n";
  char *temp;
  long cmd = strtol(argv[1], &temp, 10);
  switch (cmd) {
    case 0: // kill all
    { // explicit wrapping
      for (int i=0; i<ledger.size(); i++) {
        Send(ledger.at(i), 10);
      };
      break;
    }
    case 1: // create
    { // explicit wrapping
      std::cout << "Spawn called" << "\n";
      for (int i=0; i<3; i++) {
        Spawn(
          /** ledger */
          ledger,
          /** node */
          cfg
        );
      };
      break;
    }
    case 2: // output ledger in mem
    { // explicit wrapping
      std::cout << "SIZE: "<< ledger.size() << "\n";
      for (int i=0; i<ledger.size(); i++) {
        std::cout << "Test" << "\n";
        std::cout << ledger.at(i) << "\n";
      };
      break;
    }
  }
  return 0;
}
