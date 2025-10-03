#ifndef EXPR_H
#define EXPR_H
#include "../parser/token.hpp"
#include <string>

// Forward declarations
class Binary;
class Unary;
class Lit;

class ExprVisitor {
public:
  virtual std::string visit(class Binary *expr) = 0;
  virtual std::string visit(class Unary *expr) = 0;
  virtual std::string visit(class Lit *expr) = 0;
};

class Expr {
public:
  virtual ~Expr() = default;
  virtual std::string accept(ExprVisitor *visitor) = 0;
};

class Binary : public Expr {
public:
  Expr *left;
  Expr *right;
  Token op;

  Binary(Expr *left, Expr *right, Token op)
      : left(left), right(right), op(op) {}

  std::string accept(ExprVisitor *visitor) override {
    return visitor->visit(this);
  }
};

class Unary : public Expr {
public:
  Expr *right;
  Token op;

  Unary(Expr *right, Token op) : right(right), op(op) {}

  std::string accept(ExprVisitor *visitor) override {
    return visitor->visit(this);
  }
};

class Lit : public Expr {
public:
  std::string value;
  TokenType type;

  Lit(std::string value, TokenType type = TokenType::NUMBER)
      : value(value), type(type) {}

  std::string accept(ExprVisitor *visitor) override {
    return visitor->visit(this);
  }
};

#endif
