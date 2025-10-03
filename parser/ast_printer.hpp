#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include "../ast/expr.hpp"
#include <string>

class AstPrinter : public ExprVisitor {
public:
  std::string print(Expr *expr);
  std::string visit(Binary *expr) override;
  std::string visit(Unary *expr) override;
  std::string visit(Lit *expr) override;

private:
  std::string parenthesize(const std::string &name,
                           std::initializer_list<Expr *> exprs);
};

#endif
