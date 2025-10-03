#include "compiler.hpp"
#include "../logger.hpp"
#include "../vm/opcode.hpp"
#include <cstddef>
#include <cstdlib>

CodeObject *Compiler::compile(const Expr &expr) {
  co = AS_CODE(ALLOC_CODE("main"));
  compile_expr(expr);
  emit(OP_HALT);
  return co;
}

#define ALLOC_CONST(tester, converter, alllocator, value)                      \
  do {                                                                         \
    for (auto i = 0; i < co->constants.size(); i++) {                          \
      if (!tester(co->constants[i])) {                                         \
        continue;                                                              \
      }                                                                        \
      if (converter(co->constants[i]) == value) {                              \
        return i;                                                              \
      }                                                                        \
    }                                                                          \
    co->constants.push_back(alllocator(value));                                \
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

void Compiler::emit(uint8_t code) { co->code.push_back(code); }
void Compiler::compile_expr(const Expr &expr) { expr.accept(*this); }

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
  default:
    DIE << "todo binary " << expr.op.literal << "\n";
    exit(EXIT_SUCCESS);
  }
}

void Compiler::visit(const Unary &expr) {}

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
