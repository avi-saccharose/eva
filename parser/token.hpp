#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>
enum class TokenType {
  STRING,
  NUMBER,

  LPAREN,
  RPAREN,

  ADD,
  SUB,
  MUL,
  DIV,

  LT,
  LTEQ,
  GT,
  GTEQ,
  EQ,
  EQEQ,
  BANG,
  NTEQ,

  DEF,
  VAR,
  TRUE,
  FALSE,
  IDENT,
  eof,
};

class Token {
public:
  Token(TokenType type, std::string literal) : type(type), literal(literal) {}
  TokenType type;
  std::string literal;

  std::string type_to_string() {
    switch (type) {
    case TokenType::STRING:
      return "STRING";
    case TokenType::NUMBER:
      return "NUMBER";
    case TokenType::LPAREN:
      return "LPAREN";
    case TokenType::RPAREN:
      return "RPAREN";
    case TokenType::ADD:
      return "ADD";
    case TokenType::SUB:
      return "SUB";
    case TokenType::MUL:
      return "MUL";
    case TokenType::DIV:
      return "DIV";
    case TokenType::LT:
      return "LT";
    case TokenType::LTEQ:
      return "LTEQ";
    case TokenType::GT:
      return "GT";
    case TokenType::GTEQ:
      return "GTEQ";
    case TokenType::EQ:
      return "EQ";
    case TokenType::EQEQ:
      return "EQEQ";
    case TokenType::BANG:
      return "BANG";
    case TokenType::NTEQ:
      return "NTEQ";
    case TokenType::DEF:
      return "DEF";
    case TokenType::VAR:
      return "VAR";
    case TokenType::TRUE:
      return "TRUE";
    case TokenType::FALSE:
      return "FALSE";
    case TokenType::IDENT:
      return "IDENT";
    case TokenType::eof:
      return "EOF";
    }
  }
};

#endif
