#include "hdls.hpp"

Filler::Filler(int fint) {
   std::cout << "Filler created \n";
   this->ft = fint;
};
void Filler::Start() {
  std::cout << "Start \n";
};
void Filler::Stop() {
  std::cout << "Stop \n";
};