#include "eva_vm.hpp"

#include "../logger.hpp"
#include "eva_value.hpp"
#include "opcode.hpp"

EvaValue EvaVm::exec(const std::string &program) {
  auto ast = parser->parse(program);
  co = compiler->compile(*ast[0]);
  ip = &co->code[0];
  sp = &stack[0];
  return eval();
}

EvaValue EvaVm::eval() {
#define READ_BYTE() *ip++
#define READ_SHORT() (ip += 2, ((uint16_t)(ip[-2] << 8) | ip[-1]))
#define GET_CONST() co->constants[READ_BYTE()]
#define TO_ADDRESS(index) &co->code[index]

#define OPERANDS_EQUAL(checker, op1, op2) (checker(op1) && checker(op2))
#define BINARY_OP(op)            \
  do {                           \
    auto op1 = AS_NUMBER(pop()); \
    auto op2 = AS_NUMBER(pop()); \
    push(NUMBER(op2 op op1));    \
  } while (false)

#define COMPARE_VALUES(op, a, b) \
  do {                           \
    bool res;
    switch (op) {                \
      case OP_EQUAL:
        res = a == b;
        break;
      case OP_GREATER:
        res = a > b;
        break;
      case OP_LESS:
        res = a < b;
        break;
    }
    push(BOOLEAN(res));          \
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
      case OP_EQUAL:
      case OP_LESS:
      case OP_GREATER: {
        auto op2 = pop();
        auto op1 = pop();
        if (OPERANDS_EQUAL(IS_NUMBER, op1, op2)) {
          auto a = AS_NUMBER(op1);
          auto b = AS_NUMBER(op2);
          COMPARE_VALUES(opcode, a, b);
        }
        if (OPERANDS_EQUAL(IS_STRING, op1, op2)) {
          auto a = AS_STRING(op1);
          auto b = AS_STRING(op2);
          COMPARE_VALUES(opcode, a, b);
        }
        break;
      }
      // TODO: handle non booleans
      case OP_NOT: {
        auto value = AS_BOOLEAN(pop());
        push(BOOLEAN(!value));
        break;
      }

      case OP_JMP_IF_FALSE: {
        auto cond = AS_BOOLEAN(pop());
        auto address = READ_SHORT();

        if (!cond) {
          ip = TO_ADDRESS(address);
        }
        break;
      }

      case OP_JMP: {
        auto address = READ_SHORT();
        ip = TO_ADDRESS(address);
        break;
      }

      default:
        DIE << "Unknown opcode " << std::hex << (int)opcode;
    }
  }
#undef TO_ADDRESS
#undef COMPARE_VALUES
#undef BINARY_OP
#undef GET_CONST
#undef READ_SHORT
#undef READ_BYTE
}

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