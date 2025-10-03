#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include "../ast/expr.hpp"
#include <string>

class AstPrinter : public ExprVisitor {
public:
  std::string print(Expr *expr);
  void visit(const Binary &expr) override;
  void visit(const Unary &expr) override;
  void visit(const Lit &expr) override;

private:
  std::string parenthesize(const std::string &name,
                           std::initializer_list<Expr *> exprs);
};

#endif
