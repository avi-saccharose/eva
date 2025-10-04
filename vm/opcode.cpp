#include "opcode.hpp"

#include "../logger.hpp"

std::string opcodeToString(uint8_t opcode) {
  switch (opcode) {
    case OP_HALT:
      return "HALT";
    case OP_CONST:
      return "CONST";
    case OP_ADD:
      return "ADD";
    case OP_SUB:
      return "SUB";
    case OP_DIV:
      return "DIV";
    case OP_MUL:
      return "MUL";
    case OP_EQUAL:
      return "EQUAL";
    case OP_LESS:
      return "LESS";
    case OP_GREATER:
      return "GREATER";
    case OP_NOT:
      return "NOT";
    case OP_JMP_IF_FALSE:
      return "JMP_IF_FALSE";
    case OP_JMP:
      return "JMP";
    default:
      DIE << "opcodeToString: unknown opcode " << (int)opcode;
  }
  return "unknown";
}
