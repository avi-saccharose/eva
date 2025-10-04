
#include "disassembler.hpp"

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>

#include "../logger.hpp"
#include "../vm/opcode.hpp"
void Disassembler::disassemble(CodeObject* co) {
  std::cout << "\n------------- Disassembly: " << co->name
            << " -------------\n\n";
  size_t offset = 0;

  while (offset < co->code.size()) {
    offset = disassembleInstruction(co, offset);
    std::cout << "\n";
  }
}

size_t Disassembler::disassembleInstruction(CodeObject* co, size_t offset) {
  std::ios_base::fmtflags f(std::cout.flags());
  // print offset
  std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(4)
            << offset << "    ";
  auto opcode = co->code[offset];
  switch (opcode) {
    case OP_HALT:
      return disassembleSimple(co, opcode, offset);
    case OP_CONST:
      return disassembleConst(co, opcode, offset);
    default:
      DIE << "disassembleInstruction: no disassembly for ";
  }
  std::cout.flags(f);
  return 0;
}

size_t Disassembler::disassembleSimple(CodeObject* co, uint8_t opcode,
                                       size_t offset) {
  dumpBytes(co, offset, 1);
  printOpcode(opcode);
  return offset + 1;
}

size_t Disassembler::disassembleConst(CodeObject* co, uint8_t opcode,
                                      size_t offset) {
  dumpBytes(co, offset, 2);
  printOpcode(opcode);
  auto constIndex = co->code[offset + 1];
  std::cout << (int)constIndex << "("
            << evaValueToConstantString(co->constants[constIndex]) << ")";
  return offset + 2;
}

void Disassembler::dumpBytes(CodeObject* co, size_t offset, size_t count) {
  std::ios_base::fmtflags f(std::cout.flags());
  std::stringstream ss;

  // print offset
  for (auto i = 0; i < count; i++) {
    ss << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
       << (((int)co->code[offset + 1]) & 0xff) << "  ";
  }
  std::cout << std::left << std::setfill(' ') << std::setw(12) << ss.str();
  std::cout.flags(f);
}

void Disassembler::printOpcode(uint8_t opcode) {
  std::cout << std::left << std::setfill(' ') << std::setw(20)
            << opcodeToString(opcode) << " ";
}
