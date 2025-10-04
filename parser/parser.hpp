#ifndef PARSER_HPP
#define PARSER_HPP
#include <cstddef>
#include <vector>

#include "../ast/expr.hpp"
#include "token.hpp"

class Parser {
 public:
  Parser() : current(0) {};
  std::vector<Expr*> parse(const std::string& input);

 private:
  bool isEof();
  Token previous();
  Token advance();
  Token peek();

  bool check(TokenType type);

  template <typename... Args>
  bool match(Args... types) {
    if ((check(types) || ...)) {
      advance();
      return true;
    }
    return false;
  }

  Token consume(TokenType type, const std::string& msg);

  Expr* parseExpr();
  Expr* expression();
  Expr* expressionIf();
  Expr* equality();
  Expr* comparison();
  Expr* terminal();
  Expr* factor();
  Expr* unary();
  Expr* primary();

  std::vector<Token> tokens;
  std::vector<Expr*> exprs;
  size_t current;
};

#endif
