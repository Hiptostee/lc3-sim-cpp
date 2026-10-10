#include "lc3.hpp"
#include <cstdint>
#include "control.hpp"
#include "microcode.hpp"
#include <stdexcept>
#include <iostream>
#include <iomanip>


uint16_t LC3::sext(uint16_t value, uint16_t bits) {
  uint16_t sign = (value & (1u << (bits - 1)));
  if (sign) {
    uint16_t mask = 0xFFFF << bits;
    value = mask | value;
    return value;
  }
  else if (sign == 0) {
    return value;
  }
  return 0;
}

uint16_t LC3::evaluateADDR1MUX(const ControlSignals& signal) {
  uint16_t selector = signal.ADDR1MUX;

  if (selector == 0) {
    return PC;
  }
  else if (selector == 1) {
    uint16_t sr1 = evaluateSR1MUX(signal);
    return registers[sr1];
  }

  return 0;
}

uint16_t LC3::evaluateADDR2MUX(const ControlSignals& signal) {
  uint16_t selector = signal.ADDR2MUX;
  switch (selector) {
    case 0: {
      return 0;
    }
    case 1: {
      return sext(IR & 0x003F, 6);
    }
    case 2: {
      return sext(IR & 0x01FF, 9);
    }
    case 3: {
      return sext(IR & 0x07FF, 11);
    }
  }
  return 0;
}

uint16_t LC3::evaluateSR1MUX(const ControlSignals& signal) {
  uint16_t selector = signal.SR1MUX;
  switch (selector) {
    case 0: {
      return (IR >> 9) & 0x0007; 
    }
    case 1: {
      return (IR >> 6) & 0x0007; 
    }
  }
  return 0;
}

uint16_t LC3::evaluateDRMUX(const ControlSignals& signal) {
  uint16_t selector = signal.DRMUX;
  switch (selector) {
    case 0: {
      return (IR >> 9) & 0x0007; 
    }
    case 1: {
      return 0x0007;
    }
  }
  return 0;
}

uint16_t LC3::evaluateSR2MUX() {
  uint16_t selector = (IR >> 5) & 0x0001;
  switch (selector) {
    case 0: {
      return registers[IR & 0x0007];
    }
    case 1: {
      return sext(IR & 0x001F, 5);
    }
  }
  return 0;
} 

uint16_t LC3::evaluateMARMUX(const ControlSignals& signal) {
  uint16_t selector = signal.MARMUX;
  switch (selector) {
    case 0: {
      return IR & 0x00FF;
    }
    case 1: {
      return static_cast<uint16_t>(evaluateADDR2MUX(signal) + evaluateADDR1MUX(signal));
    }
  }
  return 0;
}

uint16_t LC3::evaluatePCMUX(const ControlSignals& signal) {
  uint16_t selector = signal.PCMUX;
  switch (selector) {
    case 0: {
      return static_cast<uint16_t>(PC + 1);
    }
    case 1: {
      return BUS;
    }
    case 2: {
      return static_cast<uint16_t>(evaluateADDR1MUX(signal) + evaluateADDR2MUX(signal));
    }
  }
  return 0;
}

uint16_t LC3::evaluateALU(const ControlSignals& signal) {
  uint16_t selector = signal.ALUK;
  uint16_t A = registers[evaluateSR1MUX(signal)];
  uint16_t B = evaluateSR2MUX();
  switch (selector) {
    case 0: {
      return static_cast<uint16_t>(A + B); 
    }
    case 1: {
      return static_cast<uint16_t>(A & B); 
    }
    case 2: {
      return static_cast<uint16_t>(~A); 
    }
    case 3: {
      return static_cast<uint16_t>(A); 
    }
  }
  return 0;
}

uint16_t LC3::evaluateBUS(const ControlSignals& signal) {
  uint16_t active_gates = signal.GatePC + signal.GateMARMUX + signal.GateALU + signal.GateMDR;
  if (active_gates > 1) {
    throw std::runtime_error("Bus Contention");
  }
  if (signal.GatePC) {
    BUS = PC;
  }
  else if (signal.GateMARMUX) {
    BUS = evaluateMARMUX(signal);
  }
  else if (signal.GateMDR) {
    BUS = MDR;
  }
  else if (signal.GateALU) {
    BUS = evaluateALU(signal);
  } else {
    BUS = 0;
  }
  return BUS;
}

void LC3::latchRegisters(const ControlSignals& signal) {
  uint16_t next_PC = evaluatePCMUX(signal);
  uint16_t next_MAR = BUS;
  uint16_t next_MDR = (!signal.R_W && signal.MEM_EN) ? memory[MAR] : BUS;
  uint16_t next_IR = BUS;
  uint16_t next_REG = BUS;
  uint16_t next_DR = evaluateDRMUX(signal);
  
  if (signal.LD_PC) {
    PC = next_PC;
  }

  if (signal.LD_MAR) {
    MAR = next_MAR;
  }

  if (signal.LD_MDR) {
    MDR = next_MDR;
  }

  if (signal.LD_IR) {
    IR = next_IR;
  }

  if (signal.LD_REG) {
    registers[next_DR] = next_REG;
  }

  if (signal.MEM_EN && signal.R_W) {
      memory[MAR] = MDR;
  }

  if (signal.LD_CC) {
    if (BUS == 0) {
        CC = 0b010; 
    }
    else if (BUS & 0x8000) {
        CC = 0b100;
    }
    else {
        CC = 0b001;
    }
  }
}

void LC3::clock(const ControlSignals& signal) {
  evaluateBUS(signal);
  latchRegisters(signal);
}

void LC3::printState() const {
  std::cout << "\n========== LC-3 CPU STATE ==========\n";

  // Internal registers
  std::cout << "\n[ DATAPATH ]\n";
  std::cout << std::hex << std::setfill('0');

  std::cout << "PC:  0x" << std::setw(4) << PC  << '\n';
  std::cout << "MAR: 0x" << std::setw(4) << MAR << '\n';
  std::cout << "MDR: 0x" << std::setw(4) << MDR << '\n';
  std::cout << "IR:  0x" << std::setw(4) << IR  << '\n';
  std::cout << "BUS: 0x" << std::setw(4) << BUS << '\n';

  // General-purpose registers
  std::cout << "\n[ GENERAL REGISTERS ]\n";

  for (int i = 0; i < 8; i++) {
      std::cout << "R" << i << ": 0x"
                << std::setw(4) << registers[i]
                << "  (" << std::dec
                << static_cast<int16_t>(registers[i])
                << std::hex << ")\n";
  }

  // Condition codes
  std::cout << "\n[ CONDITION CODES ]\n";

  std::cout << "N: " << ((CC & 0b100) != 0)
            << "  Z: " << ((CC & 0b010) != 0)
            << "  P: " << ((CC & 0b001) != 0)
            << '\n';

  // Decode instruction register
  std::cout << "\n[ INSTRUCTION DECODE ]\n";

  uint16_t opcode = (IR >> 12) & 0xF;
  uint16_t dr     = (IR >> 9) & 0x7;
  uint16_t sr1    = (IR >> 6) & 0x7;
  uint16_t sr2    = IR & 0x7;
  uint16_t imm5   = IR & 0x1F;
  bool immediate  = (IR >> 5) & 1;

  std::cout << "Opcode: 0x" << opcode << '\n';
  std::cout << "DR:     R" << dr << '\n';
  std::cout << "SR1:    R" << sr1 << '\n';

  if (immediate) {
      int16_t signedImm5 =
          (imm5 & 0x10) ? static_cast<int16_t>(imm5) - 32
                        : static_cast<int16_t>(imm5);

      std::cout << "Mode:   Immediate\n";
      std::cout << "imm5:   #" << std::dec << signedImm5 << std::hex << '\n';
  } else {
      std::cout << "Mode:   Register\n";
      std::cout << "SR2:    R" << sr2 << '\n';
  }

  std::cout << "\n====================================\n";
}

void LC3::loadWord(uint16_t addr, uint16_t value) {
  memory[addr] = value;
}

void LC3::step() {
  switch (microstate) {
    case Microstate::FETCH0: {
      clock(Microcode::getFetch0());
      microstate = Microstate::FETCH1;
      break;
    }
    case Microstate::FETCH1: {
      clock(Microcode::getFetch1());
      microstate = Microstate::FETCH2;
      break;
    }
    case Microstate::FETCH2: {
      clock(Microcode::getFetch2());
      microstate = Microstate::DECODE;
      break;
    }
    case Microstate::DECODE: {
      uint16_t opcode = (IR >> 12) & 0x000F;
      switch (opcode) {
        case 0x0001:
          microstate = Microstate::ADD;
          break;
        case 0x0005:
          microstate = Microstate::AND;
          break;
        case 0x0009:
          microstate = Microstate::NOT;
          break;
        case 0x000F:
          microstate = Microstate::HALT;
          break;
        case 0x0002:
          microstate = Microstate::LD0;
          break;
        case 0x0006:
          microstate = Microstate::LDR0;
          break;
        case 0x000e:
          microstate = Microstate::LEA;
          break;
        case 0x000A:
          microstate = Microstate::LDI0;
          break;
        case 0x3:
          microstate = Microstate::ST0;
          break;
        case 0x7:
          microstate = Microstate::STR0;
          break;
        case 0xB:
          microstate = Microstate::STI0;
          break;
        default:
          throw std::runtime_error("Unsupported opcode");
      }
      break; 
    }

    case Microstate::ADD: {
      clock(Microcode::getAdd());
      microstate = Microstate::FETCH0;
      break;
    }
    case Microstate::AND: {
      clock(Microcode::getAnd());
      microstate = Microstate::FETCH0;
      break;
    }
    case Microstate::NOT: {
      clock(Microcode::getNot());
      microstate = Microstate::FETCH0;
      break;
    }
    case Microstate::HALT: {
      halted = true;
      break;
    }
    case Microstate::LD0: {
      clock(Microcode::getLd0());
      microstate = Microstate::LD1;
      break;
    }
    case Microstate::LD1: {
      clock(Microcode::getLd1());
      microstate = Microstate::LD2;
      break;
    }
    case Microstate::LD2: {
      clock(Microcode::getLd2());
      microstate = Microstate::FETCH0;
      break;
    }
    case Microstate::LDR0: {
      clock(Microcode::getLdr0());
      microstate = Microstate::LDR1;
      break;
    }
    case Microstate::LDR1: {
      clock(Microcode::getLdr1());
      microstate = Microstate::LDR2;
      break;
    }
    case Microstate::LDR2: {
      clock(Microcode::getLdr2());
      microstate = Microstate::FETCH0;
      break;
    }
    case Microstate::LDI0: {
      clock(Microcode::getLdi0());
      microstate = Microstate::LDI1;
      break;
    }
    case Microstate::LDI1: {
      clock(Microcode::getLdi1());
      microstate = Microstate::LDI2;
      break;
    }
    case Microstate::LDI2: {
      clock(Microcode::getLdi2());
      microstate = Microstate::LDI3;
      break;
    }
    case Microstate::LDI3: {
      clock(Microcode::getLdi3());
      microstate = Microstate::LDI4;
      break;
    }
    case Microstate::LDI4: {
      clock(Microcode::getLdi4());
      microstate = Microstate::FETCH0;
      break;
    }
    case Microstate::LEA: {
      clock(Microcode::getLea());
      microstate = Microstate::FETCH0;
      break;
    }
      case Microstate::ST0: {
      clock(Microcode::getSt0());
      microstate = Microstate::ST1;
      break;
    }
    case Microstate::ST1: {
      clock(Microcode::getSt1());
      microstate = Microstate::ST2;
      break;
    }
    case Microstate::ST2: {
      clock(Microcode::getSt2());
      microstate = Microstate::FETCH0;
      break;
    }
    case Microstate::STR0: {
      clock(Microcode::getStr0());
      microstate = Microstate::STR1;
      break;
    }
    case Microstate::STR1: {
      clock(Microcode::getStr1());
      microstate = Microstate::STR2;
      break;
    }
    case Microstate::STR2: {
      clock(Microcode::getStr2());
      microstate = Microstate::FETCH0;
      break;
    }
    case Microstate::STI0: {
      clock(Microcode::getSti0());
      microstate = Microstate::STI1;
      break;
    }
    case Microstate::STI1: {
      clock(Microcode::getSti1());
      microstate = Microstate::STI2;
      break;
    }
    case Microstate::STI2: {
      clock(Microcode::getSti2());
      microstate = Microstate::STI3;
      break;
    }
    case Microstate::STI3: {
      clock(Microcode::getSti3());
      microstate = Microstate::STI4;
      break;
    }
    case Microstate::STI4: {
      clock(Microcode::getSti4());
      microstate = Microstate::FETCH0;
      break;
    }
  }
}

void LC3::run() {
  while (!halted) {
    step();
  }
}

void LC3::readWord(uint16_t addr) const {
  std::cout << addr << ": " << memory[addr] << std::endl;
}
