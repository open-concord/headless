#include "../src/hdls.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <chrono>

#include <stdlib.h> // strtol
// g++ src/hdls.hpp src/hdls.cpp src/filler.cpp test/example.cpp -o test.out -lpthread
// ^ from project root

// dummy config
config cfg = {6};

std::vector<int> ledger;

int main (int argc, char** argv) { // 0 to kill, 1 to spawn
  char *temp;
  std::cout << "Spawn called" << "\n";
  Spawn(cfg);
  return 0;
}
