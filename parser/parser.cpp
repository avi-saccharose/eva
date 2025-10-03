#include "parser.hpp"
#include "../logger.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include <cstdlib>
#include <string>
#include <vector>

std::vector<Expr *> Parser::parse(const std::string &input) {
  auto lexer = Lexer(input);
  tokens = lexer.tokenize();
  while (!isEof()) {
    auto expr = expression();
    exprs.push_back(expr);
  }

  return exprs;
}

Expr *Parser::parseExpr() { return primary(); }

Expr *Parser::expression() { return equality(); }

Expr *Parser::equality() {
  auto expr = comparison();
  while (match(TokenType::EQEQ, TokenType::NTEQ)) {
    auto op = previous();
    auto right = comparison();
    expr = new Binary(expr, right, op);
  }
  return expr;
}

Expr *Parser::comparison() {
  auto expr = terminal();
  while (
      match(TokenType::GT, TokenType::GTEQ, TokenType::LT, TokenType::LTEQ)) {
    auto op = previous();
    auto right = terminal();
    expr = new Binary(expr, right, op);
  }
  return expr;
}

Expr *Parser::terminal() {
  auto expr = factor();
  while (match(TokenType::ADD, TokenType::SUB)) {
    auto op = previous();
    auto right = factor();
    expr = new Binary(expr, right, op);
  }
  return expr;
}

Expr *Parser::factor() {
  auto expr = unary();
  while (match(TokenType::MUL, TokenType::DIV)) {
    auto op = previous();
    auto right = unary();
    expr = new Binary(expr, right, op);
  }
  return expr;
}

Expr *Parser::unary() {
  if (match(TokenType::SUB, TokenType::BANG)) {
    auto op = previous();
    auto right = unary();
    return new Unary(right, op);
  }
  return primary();
}

Expr *Parser::primary() {
  if (match(TokenType::NUMBER)) {
    auto lit = previous().literal;
    return new Lit(lit);
  }
  if (match(TokenType::IDENT)) {
    auto lit = previous().literal;
    return new Lit(lit, TokenType::IDENT);
  }

  DIE << "Unexpected character " << previous().literal << "\n";
  exit(EXIT_FAILURE);
}

Token Parser::advance() {
  if (!isEof()) {
    current++;
  }
  return previous();
}

Token Parser::previous() { return tokens.at(current - 1); }

bool Parser::check(TokenType type) {
  if (isEof()) {
    return false;
  }
  return peek().type == type;
}

Token Parser::peek() { return tokens.at(current); }

bool Parser::isEof() { return current >= tokens.size(); }
