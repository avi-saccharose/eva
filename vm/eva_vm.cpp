
#include "eva_vm.hpp"

#include "../logger.hpp"
#include "opcode.hpp"

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
  auto ast = parser->parse(program);
  co = compiler->compile(*ast[0]);
  ip = &co->code[0];
  sp = &stack[0];
  return eval();
}

EvaValue EvaVm::eval() {
#define READ_BYTE() *ip++
#define GET_CONST() co->constants[READ_BYTE()]

#define BINARY_OP(op)            \
  do {                           \
    auto op1 = AS_NUMBER(pop()); \
    auto op2 = AS_NUMBER(pop()); \
    push(NUMBER(op2 op op1));    \
  } while (false)

  for (;;) {
    auto opcode = READ_BYTE();
    switch (opcode) {
      case OP_HALT:
        return pop();
      case OP_CONST:
        push(GET_CONST());
        break;
      case OP_ADD: {
        auto op1 = pop();
        auto op2 = pop();

        // Numbers
        if (IS_NUMBER(op1) && IS_NUMBER(op2)) {
          auto a = AS_NUMBER(op1);
          auto b = AS_NUMBER(op2);
          push(NUMBER(b + a));
        }

        if (IS_STRING(op1) && IS_STRING(op2)) {
          auto s1 = AS_CPPSTRING(op1);
          auto s2 = AS_CPPSTRING(op2);
          push(ALLOC_STRING(s2 + s1));
        }
        break;
      }

      case OP_SUB: {
        BINARY_OP(-);
        break;
      }
      case OP_DIV: {
        BINARY_OP(/);
        break;
      }
      case OP_MUL: {
        BINARY_OP(*);
        break;
      }
      default:
        DIE << "Unknown opcode " << std::hex << (int)opcode;
    }
  }
#undef BINARY_OP
#undef GET_CONST
#undef READ_BYTE
}
