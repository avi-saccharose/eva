#ifndef EXPR_H
#define EXPR_H

#include <string>

#include "../parser/token.hpp"

// Forward declarations
class Binary;
class Unary;
class Lit;

class ExprVisitor {
 public:
  virtual ~ExprVisitor() = default;
  virtual void visit(const class Binary &expr) = 0;
  virtual void visit(const class Unary &expr) = 0;
  virtual void visit(const class Lit &expr) = 0;
  virtual void visit(const class If &expr) = 0;
};

class Expr {
 public:
  virtual ~Expr() = default;

  virtual void accept(class ExprVisitor &visitor) const = 0;
};

class If : public Expr {
 public:
  Expr *cond;
  Expr *then_branch;
  Expr *else_branch;

  If(Expr *cond, Expr *then_branch, Expr *else_branch)
      : cond(cond), then_branch(then_branch), else_branch(else_branch) {}

  void accept(class ExprVisitor &visitior) const override {
    return visitior.visit(*this);
  }
};

class Binary : public Expr {
 public:
  Expr *left;
  Expr *right;
  Token op;

  Binary(Expr *left, Expr *right, Token op)
      : left(left), right(right), op(op) {}

  void accept(class ExprVisitor &visitor) const override {
    return visitor.visit(*this);
  }
};

class Unary : public Expr {
 public:
  Expr *right;
  Token op;

  Unary(Expr *right, Token op) : right(right), op(op) {}
  void accept(class ExprVisitor &visitor) const override {
    return visitor.visit(*this);
  }
};

class Lit : public Expr {
 public:
  std::string value;
  TokenType type;

  Lit(std::string value, TokenType type = TokenType::NUMBER)
      : value(value), type(type) {}

  void accept(class ExprVisitor &visitor) const override {
    return visitor.visit(*this);
  }
};

#endif
