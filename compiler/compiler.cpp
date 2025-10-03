#include "compiler.hpp"
#include "../logger.hpp"
#include "../vm/opcode.hpp"
#include <cstddef>
#include <cstdlib>

CodeObject *Compiler::compile(const Expr &expr) {
  co = AS_CODE(ALLOC_CODE("main"));
  visit_expr(expr);
  emit(OP_HALT);
  return co;
}

size_t Compiler::numericConstIdx(double value) {
  for (auto i = 0; i < co->constants.size(); i++) {
    if (!IS_NUMBER(co->constants[i])) {
      continue;
    }
    if (AS_NUMBER(co->constants[i]) == value) {
      return i;
    }
  }
  co->constants.push_back(NUMBER(value));
  return co->constants.size() - 1;
}

size_t Compiler::stringConstIdx(const std::string &value) {
  for (auto i = 0; i < co->constants.size(); i++) {
    if (!IS_STRING(co->constants[i])) {
      continue;
    }
    if (AS_CPPSTRING(co->constants[i]) == value) {
      return i;
    }
  }
  co->constants.push_back(ALLOC_STRING(value));
  return co->constants.size() - 1;
}

void Compiler::emit(uint8_t code) { co->code.push_back(code); }
void Compiler::visit_expr(const Expr &expr) { expr.accept(*this); }
void Compiler::visit(const Binary &expr) {}

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

  default:
    DIE << "TODO: " << expr.value << "\n";
    exit(EXIT_FAILURE);
  }
}
