#include "../inc/hdls.hpp"
#include <iostream>

// dummy config
config cfg = {6};

int main (int argc, char** argv) { // 0 to kill, 1 to spawn
  std::cout << "Spawn called" << "\n";
  Spawn(cfg);
  return 0;
}
