#include "eva_vm.hpp"
#include "eva_value.hpp"
#include "logger.hpp"
#include "opcode.hpp"

#define READ_BYTE() *ip++
#define GET_CONST() constants[READ_BYTE()]

void EvaVm::push(const EvaValue &value) {
  if ((size_t)(sp - stack.begin()) == STACK_LIMIT) {
    DIE << "Stack overflow.\n";
  }
  *sp = value;
  sp++;
}

EvaValue EvaVm::pop() {
  if (sp == stack.begin()) {
    DIE << "pop(): empty stack.\n";
  }
  sp--;
  return *sp;
}

EvaValue EvaVm::exec(const std::string &program) {
  constants.push_back(NUMBER(42));

  code = {OP_CONST, 0, OP_HALT};
  ip = &code[0];
  return eval();
}

EvaValue EvaVm::eval() {
  for (;;) {
    auto opcode = READ_BYTE();
    switch (opcode) {
    case OP_HALT:
      return pop();
    case OP_CONST:
      push(GET_CONST());
      break;
    default:
      DIE << "Unknown opcode " << std::hex << (int)opcode;
    }
  }
}
