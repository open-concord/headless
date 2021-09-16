#include <unistd.h> // pid
#include <iostream>
#include <string>

pid_t outside_function() {
  return getpid();
}

int main(int argc, char const *argv[]) {
  std::cout << std::to_string(getpid()) << ": " << sizeof(std::to_string(getpid())) << "\n";
  return 0;
};
