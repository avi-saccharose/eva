#ifndef EVA_VM_H
#define EVA_VM_H
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "eva_value.hpp"

#define STACK_LIMIT 512

class EvaVm {
public:
  EvaVm() { sp = &stack[0]; }
  EvaValue exec(const std::string &program);

  EvaValue eval();

  void push(const EvaValue &value);
  EvaValue pop();

  uint8_t *ip;
  EvaValue *sp;

  std::array<EvaValue, STACK_LIMIT> stack;
  std::vector<uint8_t> code;
  std::vector<EvaValue> constants;
};
#endif
