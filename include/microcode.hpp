#pragma once
#include "control.hpp"

namespace Microcode {

inline ControlSignals getFetch0() {
  ControlSignals c;
  c.GatePC = true;
  c.LD_MAR = true;
  c.LD_PC = true;
  c.PCMUX = 0;
  return c;
}

inline ControlSignals getFetch1() {
  ControlSignals c;
  c.MEM_EN = true;
  c.LD_MDR = true;
  return c;
}

inline ControlSignals getFetch2() {
  ControlSignals c;
  c.LD_IR = true;
  c.GateMDR = true;
  return c;
}

inline ControlSignals getAdd() {
  ControlSignals c;
  c.SR1MUX = 1;
  c.GateALU = true;
  c.ALUK = 0;
  c.LD_REG = true;
  c.LD_CC = true;
  return c;
}

inline ControlSignals getAnd() {
  ControlSignals c;
  c.GateALU = true;
  c.ALUK = 1;
  c.LD_REG = true;
  c.LD_CC = true;
  return c;
}

inline ControlSignals getNot() {
  ControlSignals c;
  c.GateALU = true;
  c.ALUK = 2;
  c.LD_REG = true;
  c.LD_CC = true;
  return c;
}


}


