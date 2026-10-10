#include <iostream>
#include "lc3.hpp"
#include "microcode.hpp"

int main() {
  LC3 cpu;
  cpu.loadWord(0x3000, 0x1265);
  cpu.loadWord(0x3001, 0x1265);
  cpu.loadWord(0x3002, 0x1265);
  cpu.loadWord(0x3003, 0xF025);
  cpu.run();
  cpu.printState();
  return 0;
}
