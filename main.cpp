#include "logger.hpp"
#include "vm/eva_value.hpp"
#include "vm/eva_vm.hpp"

// #include "parser/ast_printer.hpp"

#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
  EvaVm vm;
  auto result = vm.exec(R"(
                      3
  )");

  log(AS_NUMBER(result));

  std::cout << "Program executed successfully\n";

  return 0;
}
