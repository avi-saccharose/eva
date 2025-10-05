#include "compiler.hpp"

#include <cstddef>
#include <cstdlib>

#include "../logger.hpp"
#include "../vm/opcode.hpp"

CodeObject *Compiler::compile(const Expr &expr) {
  co = AS_CODE(ALLOC_CODE("main"));
  compile_expr(expr);
  emit(OP_HALT);
  return co;
}

void Compiler::compile_expr(const Expr &expr) { expr.accept(*this); }

// Compile if expressions
void Compiler::visit(const If &expr) {
  // Compile the condition
  expr.cond->accept(*this);

  emit(OP_JMP_IF_FALSE);
  // store placeholder address for else branch
  emit(0);
  emit(0);
  auto elseJmpAddr = getOffset() - 2;

  // Compile the then branch
  expr.then_branch->accept(*this);

  emit(OP_JMP);
  emit(0);
  emit(0);

  auto endAddr = getOffset() - 2;

  auto elseBranchAddr = getOffset();
  patchJumpAddress(elseJmpAddr, elseBranchAddr);

  // Compile else branch
  expr.else_branch->accept(*this);

  auto endBranchAddr = getOffset();
  patchJumpAddress(endAddr, endBranchAddr);
}

// Compile Binary expressions
void Compiler::visit(const Binary &expr) {
  expr.left->accept(*this);
  expr.right->accept(*this);

  switch (expr.op.type) {
    case TokenType::ADD:
      emit(OP_ADD);
      break;
    case TokenType::SUB:
      emit(OP_SUB);
      break;
    case TokenType::MUL:
      emit(OP_MUL);
      break;
    case TokenType::DIV:
      emit(OP_DIV);
      break;
    case TokenType::EQEQ:
      emit(OP_EQUAL);
      break;
    case TokenType::NTEQ:
      emit(OP_EQUAL);
      emit(OP_NOT);
      break;
    case TokenType::LT:
      emit(OP_LESS);
      break;
    case TokenType::LTEQ:
      emit(OP_GREATER);
      emit(OP_NOT);
      break;
    case TokenType::GT:
      emit(OP_GREATER);
      break;
    case TokenType::GTEQ:
      emit(OP_LESS);
      emit(OP_NOT);
      break;
    default:
      DIE << "todo binary op " << expr.op.literal << "\n";
      exit(EXIT_SUCCESS);
  }
}

// TODO: compile unary expressions
void Compiler::visit(const Unary &expr) {}

// Compile Literal expressions
void Compiler::visit(const Lit &expr) {
  switch (expr.type) {
    case TokenType::NUMBER: {
      emit(OP_CONST);
      auto value = std::stod(expr.value);
      emit(numericConstIdx(value));
      break;
    }
    case TokenType::STRING:
      emit(OP_CONST);
      emit(stringConstIdx(expr.value));
      break;
    case TokenType::TRUE:
      emit(OP_CONST);
      emit(boolConstIdx(true));
      break;
    case TokenType::FALSE:
      emit(OP_CONST);
      emit(boolConstIdx(false));
      break;
    default:
      DIE << "TODO: " << expr.value << "\n";
      exit(EXIT_FAILURE);
  }
}

void Compiler::disassembleBytecode() { disassembler->disassemble(co); }

void Compiler::emit(uint8_t code) { co->code.push_back(code); }

void Compiler::writeByteAtOffset(size_t offset, uint8_t value) {
  co->code[offset] = value;
}

void Compiler::patchJumpAddress(size_t offset, uint16_t value) {
  writeByteAtOffset(offset, (value >> 8) & 0xff);
  writeByteAtOffset(offset + 1, value & 0xff);
}

size_t Compiler::getOffset() { return co->code.size(); }

#define ALLOC_CONST(tester, converter, alllocator, value) \
  do {                                                    \
    for (auto i = 0; i < co->constants.size(); i++) {     \
      if (!tester(co->constants[i])) {                    \
        continue;                                         \
      }                                                   \
      if (converter(co->constants[i]) == value) {         \
        return i;                                         \
      }                                                   \
    }                                                     \
    co->constants.push_back(alllocator(value));           \
  } while (false)

size_t Compiler::numericConstIdx(double value) {
  ALLOC_CONST(IS_NUMBER, AS_NUMBER, NUMBER, value);
  return co->constants.size() - 1;
}

size_t Compiler::stringConstIdx(const std::string &value) {
  ALLOC_CONST(IS_STRING, AS_CPPSTRING, ALLOC_STRING, value);
  return co->constants.size() - 1;
}

size_t Compiler::boolConstIdx(const bool &value) {
  ALLOC_CONST(IS_BOOLEAN, AS_BOOLEAN, BOOLEAN, value);
  return co->constants.size() - 1;
}
#undef ALLOC_CONST
