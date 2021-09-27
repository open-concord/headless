#include "hdls.hpp"
#include <chrono>

Filler::Filler(int fint) {
   std::cout << "Filler created \n";
   this->ft = fint;
};
void Filler::Start() {
  std::cout << "Start \n";
  while(1) {
    // running
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << getpid() << ": Running" << "\n";
  };
};

void Filler::Stop() {
  std::cout << "Stop \n";
};