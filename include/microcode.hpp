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
  c.SR1MUX = 1;
  c.ALUK = 2;
  c.LD_REG = true;
  c.LD_CC = true;
  return c;
}

inline ControlSignals getLd0() {
  ControlSignals c;
  c.ADDR2MUX = 2;
  c.MARMUX = 1;
  c.GateMARMUX = true;
  c.LD_MAR = true;
  return c; 
}

inline ControlSignals getLd1() {
  ControlSignals c;
  c.LD_MDR = true;
  c.MEM_EN = true;
  return c; 
}

inline ControlSignals getLd2() {
  ControlSignals c;
  c.GateMDR = true;
  c.LD_CC = true;
  c.LD_REG = true;
  return c; 
}

inline ControlSignals getLdr0() {
  ControlSignals c;
  c.ADDR2MUX = 1;
  c.ADDR1MUX = 1;
  c.SR1MUX = 1;
  c.MARMUX = 1;
  c.GateMARMUX = true;
  c.LD_MAR = true;
  return c;
}

inline ControlSignals getLdr1() {
  ControlSignals c;
  c.LD_MDR = true;
  c.MEM_EN = true;
  return c;
}

inline ControlSignals getLdr2() {
  ControlSignals c;
  c.GateMDR = true;
  c.LD_CC = true;
  c.LD_REG = true;
  return c;
}

inline ControlSignals getLea() {
  ControlSignals c;
  c.ADDR2MUX = 2;
  c.GateMARMUX = true;
  c.LD_REG = true;
  c.MARMUX = 1;
  return c;
}

inline ControlSignals getLdi0() {
  ControlSignals c;
  c.ADDR2MUX = 2;
  c.GateMARMUX = true;
  c.LD_MAR = true;
  c.MARMUX = 1;
  return c;
}

inline ControlSignals getLdi1() {
  ControlSignals c;
  c.MEM_EN = true;
  c.LD_MDR = true;
  return c;
}

inline ControlSignals getLdi2() {
  ControlSignals c;
  c.GateMDR = true;
  c.LD_MAR = true;
  return c;
}

inline ControlSignals getLdi3() {
  ControlSignals c;
  c.MEM_EN = true;
  c.LD_MDR = true;
  return c;
}

inline ControlSignals getLdi4() {
  ControlSignals c;
  c.GateMDR = true;
  c.LD_CC = true;
  c.LD_REG = true;
  return c;
}

inline ControlSignals getSt0() {
  ControlSignals c;
  c.ADDR2MUX = 2;
  c.GateMARMUX = true;
  c.LD_MAR = true;
  c.MARMUX = true;
  return c;
}

inline ControlSignals getSt1() {
  ControlSignals c;
  c.ALUK = 3;
  c.GateALU = true;
  c.LD_MDR = true;
  return c;
}

inline ControlSignals getSt2() {
  ControlSignals c;
  c.MEM_EN = true;
  c.R_W = true;
  return c;
}

inline ControlSignals getStr0() {
  ControlSignals c;
  c.ADDR1MUX = 1;
  c.ADDR2MUX = 1;
  c.GateMARMUX = true;
  c.MARMUX = 1;
  c.LD_MAR = true;
  c.SR1MUX = 1;
  return c;
} 

inline ControlSignals getStr1() {
  ControlSignals c;
  c.ALUK = 3;
  c.GateALU = true;
  c.LD_MDR = true;
  return c;
} 

inline ControlSignals getStr2() {
  ControlSignals c;
  c.MEM_EN = true;
  c.R_W = true;
  return c;
}

inline ControlSignals getSti0() {
  ControlSignals c;
  c.LD_MAR = true;
  c.GateMARMUX = true;
  c.MARMUX = 1;
  c.ADDR2MUX = 2;
  return c;
}

inline ControlSignals getSti1() {
  ControlSignals c;
  c.LD_MDR = true;
  c.MEM_EN = true;
  return c;
}

inline ControlSignals getSti2() {
  ControlSignals c;
  c.GateMDR = true;
  c.LD_MAR = true;
  return c;
}

inline ControlSignals getSti3() {
  ControlSignals c;
  c.LD_MDR = true;
  c.ALUK = 3;
  c.GateALU = true;
  return c;
}

inline ControlSignals getSti4() {
  ControlSignals c;
  c.MEM_EN = true;
  c.R_W = true;
  return c;
}
}


