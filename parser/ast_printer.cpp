#include "ast_printer.hpp"
#include <vector>

// TODO: Fix this

std::string AstPrinter::print(Expr *expr) { return expr->accept(this); }

void AstPrinter::visit(const Binary &expr) {
  return parenthesize(expr.op.literal, {expr.left, expr.right});
}

void AstPrinter::visit(const Unary &expr) {
  return parenthesize(expr.op.literal, {expr.right});
}

void AstPrinter::visit(const Lit &expr) { return expr.value; }

std::string AstPrinter::parenthesize(const std::string &name,
                                     std::initializer_list<Expr *> exprs) {
  std::string result = "(" + name;
  for (Expr *expr : exprs) {
    result += " " + expr->accept(this);
  }
  result += ")";
  return result;
}
