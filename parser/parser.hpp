#ifndef PARSER_HPP
#define PARSER_HPP
#include <cstddef>
#include <vector>

#include "../ast/expr.hpp"
#include "token.hpp"

class Parser {
  std::vector<Token> tokens;
  std::vector<Expr*> exprs;
  size_t current;

  Token previous();
  Token advance();

  bool isEof();
  Token peek();

  template <typename... Args>
  bool match(Args... types) {
    if ((check(types) || ...)) {
      advance();
      return true;
    }
    return false;
  }

  bool check(TokenType type);
  Expr* parseExpr();
  Expr* expression();
  Expr* equality();
  Expr* comparison();
  Expr* terminal();
  Expr* factor();
  Expr* unary();
  Expr* primary();

 public:
  Parser() : current(0) {};
  std::vector<Expr*> parse(const std::string& input);
};

#endif
