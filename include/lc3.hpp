#pragma once
#include "control.hpp"
#include "microstate.hpp"
#include <cstdint>

class LC3 {
private:
  uint16_t memory[65536] = {};
  uint16_t registers[8] = {};
  uint16_t PC = 0x3000;
  uint16_t IR = 0;
  uint16_t CC = 0b010;
  uint16_t MAR = 0;
  uint16_t MDR = 0;
  uint16_t BUS = 0;
  bool halted = false;
  Microstate microstate = Microstate::FETCH0;

public:
  void clock(const ControlSignals& signal);
  void printState() const;
  void loadWord(uint16_t addr, uint16_t value);
  void readWord(uint16_t addr) const;
  void run();

private:
  uint16_t evaluateALU(const ControlSignals& signal);
  uint16_t evaluateADDR1MUX(const ControlSignals& signal);
  uint16_t evaluateADDR2MUX(const ControlSignals& signal);
  uint16_t evaluateSR1MUX(const ControlSignals& signal);
  uint16_t evaluateSR2MUX();
  uint16_t evaluatePCMUX(const ControlSignals& signal);
  uint16_t evaluateDRMUX(const ControlSignals& signal);
  uint16_t evaluateMARMUX(const ControlSignals& signal);
  uint16_t sext(uint16_t value, uint16_t bits);
  uint16_t evaluateBUS(const ControlSignals& signal);
  void latchRegisters(const ControlSignals& signal);
  void step();
};
