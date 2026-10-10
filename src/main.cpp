#include "lc3.hpp"
#include <iostream>
#include <cstdint>

int main() {
  LC3 cpu;

  cpu.loadWord(0x3000, 0xE81F);
  cpu.loadWord(0x3001, 0x5260);
  cpu.loadWord(0x3002, 0x126D);
  cpu.loadWord(0x3003, 0x7301);
  cpu.loadWord(0x3004, 0x6501);
  cpu.loadWord(0x3005, 0x96BF);
  cpu.loadWord(0x3006, 0x3619);
  cpu.loadWord(0x3007, 0x2A18);
  cpu.loadWord(0x3008, 0xB419);
  cpu.loadWord(0x3009, 0xAC18);
  cpu.loadWord(0x300A, 0x1DBB);
  cpu.loadWord(0x300B, 0x7D3F);
  cpu.loadWord(0x300C, 0x6F3F);
  cpu.loadWord(0x300D, 0x5FE6);
  cpu.loadWord(0x300E, 0x2016);
  cpu.loadWord(0x300F, 0xF025);

  cpu.loadWord(0x3022, 0x3025);

  cpu.run();
  cpu.printState();

  std::cout << "\n[ MEMORY VERIFICATION ]\n";
  for (uint16_t addr : {0x301F, 0x3020, 0x3021, 0x3022, 0x3025}) {
    cpu.readWord(addr);
  }

  return 0;
}
