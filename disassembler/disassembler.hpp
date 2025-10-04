#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H
#include <cstdint>

#include "../vm/eva_value.hpp"

class Disassembler {
 public:
  void disassemble(CodeObject* co);

 private:
  size_t disassembleInstruction(CodeObject* co, size_t offset);
  size_t disassembleSimple(CodeObject* co, uint8_t opcode, size_t offset);
  size_t disassembleConst(CodeObject* co, uint8_t opcode, size_t offset);
  void dumpBytes(CodeObject* co, size_t offset, size_t count);
  void printOpcode(uint8_t opcode);
};
#endif
