#include "parser.hpp"

#include <cstdlib>
#include <string>
#include <vector>

#include "../logger.hpp"
#include "lexer.hpp"
#include "token.hpp"

std::vector<Expr *> Parser::parse(const std::string &input) {
  auto lexer = Lexer(input);
  tokens = lexer.tokenize();
  while (!isEof()) {
    auto expr = expression();
    exprs.push_back(expr);
  }

  return exprs;
}

bool Parser::isEof() { return tokens.size() <= current; }

Token Parser::previous() { return tokens.at(current - 1); }

Token Parser::advance() {
  if (!isEof()) {
    current++;
  }
  return previous();
}

Token Parser::peek() { return tokens.at(current); }

bool Parser::check(TokenType type) {
  if (isEof()) {
    return false;
  }
  return peek().type == type;
}

Token Parser::consume(TokenType type, const std::string &msg) {
  if (check(type)) {
    return advance();
  }
  DIE << msg << "\n";
  exit(EXIT_FAILURE);
}

Expr *Parser::parseExpr() { return primary(); }

Expr *Parser::expression() {
  if (match(TokenType::IF)) {
    return expressionIf();
  }
  return equality();
}

Expr *Parser::expressionIf() {
  consume(TokenType::LPAREN, "Expected '(' after if");
  auto cond = expression();
  consume(TokenType::RPAREN, "Expected ')' after condition");
  auto then_branch = expression();
  consume(TokenType::ELSE, "Expected else after expression");
  auto else_branch = expression();

  return new If(cond, then_branch, else_branch);
}

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
  if (match(TokenType::LPAREN)) {
    auto expr = expression();
    consume(TokenType::RPAREN, "Expected ')' after expression");
    return expr;
  }

  if (match(TokenType::NUMBER)) {
    auto lit = previous().literal;
    return new Lit(lit);
  }

  if (match(TokenType::IDENT, TokenType::STRING, TokenType::TRUE,
            TokenType::FALSE)) {
    auto type = previous().type;
    auto lit = previous().literal;
    return new Lit(lit, type);
  }
  if (isEof()) {
    DIE << "Expected literal\n";
  } else {
    DIE << "Unexpected character " << previous().literal << "\n";
  }
  exit(EXIT_FAILURE);
}