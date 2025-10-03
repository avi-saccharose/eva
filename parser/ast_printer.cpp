#include "ast_printer.hpp"
#include <vector>

std::string AstPrinter::print(Expr *expr) { return expr->accept(this); }

std::string AstPrinter::visit(Binary *expr) {
  return parenthesize(expr->op.literal, {expr->left, expr->right});
}

std::string AstPrinter::visit(Unary *expr) {
  return parenthesize(expr->op.literal, {expr->right});
}

std::string AstPrinter::visit(Lit *expr) { return expr->value; }

std::string
AstPrinter::parenthesize(const std::string &name,
                         std::initializer_list<Expr *> exprs) {
  std::string result = "(" + name;
  for (Expr *expr : exprs) {
    result += " " + expr->accept(this);
  }
  result += ")";
  return result;
}
