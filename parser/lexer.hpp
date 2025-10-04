#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstddef>
#include <vector>

#include "token.hpp"

class Lexer {
 public:
  Lexer(const std::string &source) : source(source), current(0) {}
  std::vector<Token> tokenize();

 private:
  bool isEof();
  bool isAlpha(char c);
  bool isNumeric(char c);
  bool isAlphaNumeric(char c);
  char peek(size_t index = 0);
  bool match(char c);

  char nextChar();
  void nextToken();
  void number();
  void string();
  void ident();
  TokenType getIdent(const std::string &ident);
  void addToken(TokenType type, std::string literal);

  std::string source;
  size_t current;
  char previous;
  std::vector<Token> tokens;
};

#endif  // !LEXER_HPP
