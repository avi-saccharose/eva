#ifndef EVA_VM_H
#define EVA_VM_H
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../compiler/compiler.hpp"
#include "../parser/parser.hpp"
#include "eva_value.hpp"

#define STACK_LIMIT 512

class EvaVm {
public:
  EvaVm()
      : parser(std::make_unique<Parser>()),
        compiler(std::make_unique<Compiler>()) {}

  EvaValue exec(const std::string &program);

private:
  EvaValue eval();

  void push(const EvaValue &value);
  EvaValue pop();

  uint8_t *ip;
  EvaValue *sp;

  std::array<EvaValue, STACK_LIMIT> stack;
  std::vector<EvaValue> constants;
  std::unique_ptr<Parser> parser;
  std::unique_ptr<Compiler> compiler;
  CodeObject *co;
};
#endif
