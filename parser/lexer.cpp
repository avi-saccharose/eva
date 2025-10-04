#include "lexer.hpp"

#include <cstddef>
#include <cstdlib>
#include <map>

#include "../logger.hpp"
#include "token.hpp"

std::vector<Token> Lexer::tokenize() {
  while (!isEof()) {
    nextToken();
  }
  return tokens;
}

bool Lexer::isEof() {
  if (current >= source.length()) {
    return true;
  }
  return false;
}

bool Lexer::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isNumeric(char c) { return (c >= '0' && c <= '9'); }

bool Lexer::isAlphaNumeric(char c) { return isAlpha(c) || isNumeric(c); }

char Lexer::peek(size_t index) {
  if (isEof()) {
    return '\0';
  }
  return source[current + index];
}

bool Lexer::match(char c) {
  if (isEof()) {
    return false;
  }
  if (peek(0) != c) {
    return false;
  }
  nextChar();
  return true;
}

char Lexer::nextChar() {
  if (isEof()) {
    return '\0';
  }
  previous = source[current];
  current++;
  return source[current - 1];
}

void Lexer::nextToken() {
  auto c = nextChar();

  if (isNumeric(c)) {
    return number();
  }

  if (isAlpha(c)) {
    return ident();
  }

  switch (c) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      break;
    case '"':
      return string();
    case '(': 
      return addToken(TokenType::LPAREN, "(");
    case ')':
      return addToken(TokenType::RPAREN, ")");
    case '+':
      return addToken(TokenType::ADD, "+");
    case '-':
      return addToken(TokenType::SUB, "-");
    case '/':
      return addToken(TokenType::DIV, "/");
    case '*':
      return addToken(TokenType::MUL, "*");
    case '<':
      return match('=') ? addToken(TokenType::LTEQ, "<=")
                        : addToken(TokenType::LT, "<");
    case '>':
      return match('=') ? addToken(TokenType::GTEQ, ">=")
                        : addToken(TokenType::GT, ">");
    case '!':
      return match('=') ? addToken(TokenType::NTEQ, "!=")
                        : addToken(TokenType::BANG, "!");
    case '=':
      return match('=') ? addToken(TokenType::EQEQ, "==")
                        : addToken(TokenType::EQ, "=");
    default:
      DIE << "Unexpected token " << c << "\n";
      exit(EXIT_FAILURE);
  }
}

void Lexer::number() {
  std::string literal;
  literal.push_back(previous);
  while (!isEof() && isNumeric(peek(0))) {
    literal.push_back(nextChar());
  }
  addToken(TokenType::NUMBER, literal);
}

void Lexer::string() {
  std::string literal;
  while (!isEof()) {
    literal.push_back(nextChar());
    if (match('"')) {
      return addToken(TokenType::STRING, literal);
    }
  }

  DIE << "Unterminated string\n";
  exit(EXIT_FAILURE);
}

void Lexer::ident() {
  std::string literal;
  literal.push_back(previous);
  while (!isEof() && (isAlphaNumeric(peek(0)) || peek(0) == '_')) {
    literal.push_back(nextChar());
  }
  auto type = getIdent(literal);
  addToken(type, literal);
}

TokenType Lexer::getIdent(const std::string &ident) {
  static std::map<std::string, TokenType> keywords = {
      {"def", TokenType::DEF},   {"var", TokenType::VAR},
      {"true", TokenType::TRUE}, {"false", TokenType::FALSE},
      {"if", TokenType::IF},     {"else", TokenType::ELSE},
  };
  if (keywords.contains(ident)) {
    return keywords[ident];
  }
  return TokenType::IDENT;
}

void Lexer::addToken(TokenType type, std::string literal) {
  tokens.push_back(Token(type, literal));
}