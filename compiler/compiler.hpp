#ifndef COMPILER_H
#define COMPILER_H

#include "../ast/expr.hpp"
#include "../vm/eva_value.hpp"
#include <cstdint>

class Compiler : public ExprVisitor {
public:
  void visit(const Binary &expr) override;
  void visit(const Unary &expr) override;
  void visit(const Lit &expr) override;
  void visit_expr(const Expr &expr);

  CodeObject *compile(const Expr &expr);

private:
  CodeObject *co;
  void emit(uint8_t code);
  size_t numericConstIdx(double value);
  size_t stringConstIdx(const std::string &value);
  size_t boolConstIdx(const bool &value);
};
#endif
