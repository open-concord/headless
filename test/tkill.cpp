#include "../src/hdls.hpp"

int main (int argc, char** argv) {
  char *temp;
  long target = strtol(argv[1], &temp, 10);
  
  Send(target, 10);
}