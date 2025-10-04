#ifndef OPCODE_H
#define OPCODE_H

#include <cstdint>
#include <string>
#define OP_HALT 0x00

#define OP_CONST 0x01

// Arithmetic
#define OP_ADD 0x02
#define OP_SUB 0x03
#define OP_DIV 0x04
#define OP_MUL 0x05

// Logical
#define OP_EQUAL 0x06
#define OP_LESS 0x07
#define OP_GREATER 0x08
#define OP_NOT 0x09

#define OP_JMP_IF_FALSE 0xA
#define OP_JMP 0xB

std::string opcodeToString(uint8_t opcode);

#endif
