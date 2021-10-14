#include "../inc/hdls.hpp"
#include <chrono>

Filler::Filler(int fint) {
   std::cout << "Filler created \n";
   this->ft = fint;
   this->done = false;
};
void Filler::Start() {
  while(!this->done) {
    // running
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << getpid() << ": Running" << "\n";
    std::cout << getpid() << ": fint: " << this->ft << std::endl;
  };
  std::cout << getpid() << ": Stopping" << std::endl;
};

void Filler::Stop() {
  // stop here lmfao
  this->done = true;
}
