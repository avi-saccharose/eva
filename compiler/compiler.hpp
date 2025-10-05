#ifndef COMPILER_H
#define COMPILER_H

#include <cstdint>
#include <memory>

#include "../ast/expr.hpp"
#include "../disassembler/disassembler.hpp"
#include "../vm/eva_value.hpp"

class Compiler : public ExprVisitor {
 public:
  Compiler() : disassembler(std::make_unique<Disassembler>()) {}
  CodeObject *compile(const Expr &expr);

  void compile_expr(const Expr &expr);
  void visit(const If &expr) override;
  void visit(const Binary &expr) override;
  void visit(const Unary &expr) override;
  void visit(const Lit &expr) override;

  void disassembleBytecode();

 private:
  CodeObject *co;
  std::unique_ptr<Disassembler> disassembler;
  void emit(uint8_t code);
  void writeByteAtOffset(size_t offset, uint8_t value);
  void patchJumpAddress(size_t offset, uint16_t value);
  size_t getOffset();
  size_t numericConstIdx(double value);
  size_t stringConstIdx(const std::string &value);
  size_t boolConstIdx(const bool &value);
};

#endif
