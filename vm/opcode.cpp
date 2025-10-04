#include "opcode.hpp"

#include "../logger.hpp"

std::string opcodeToString(uint8_t opcode) {
  switch (opcode) {
    case OP_HALT:
      return "HALT";
    case OP_CONST:
      return "CONST";
    default:
      DIE << "opcodeToString: unknown opcode " << (int)opcode;
  }
  return "unknown";
}
