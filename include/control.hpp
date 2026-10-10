#pragma once
#include <cstdint>

struct ControlSignals {
  bool LD_PC = false;
  bool LD_MAR = false;
  bool LD_MDR = false;
  bool LD_IR = false;
  bool LD_REG = false;
  bool LD_CC = false;
  bool GatePC = false;
  bool GateALU = false;
  bool GateMDR = false;
  bool GateMARMUX = false;
  bool MEM_EN = false;
  uint8_t PCMUX = 0;
  uint8_t ADDR1MUX = 0;
  uint8_t ADDR2MUX = 0;
  uint8_t SR1MUX = 0;
  uint8_t SR2MUX = 0;
  uint8_t MARMUX = 0;
  uint8_t DRMUX = 0;
  uint8_t ALUK = 0;
  uint8_t RW = 0;
};
