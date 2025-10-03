#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <vector>

class Parser {
  Lexer lexer;
  std::vector<Token> tokens;

public:
  Parser(std::string source) : lexer(source) { tokens = lexer.tokenize(); }
};

#endif
