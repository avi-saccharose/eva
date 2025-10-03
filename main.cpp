#include "logger.hpp"
#include "vm/eva_value.hpp"
#include "vm/eva_vm.hpp"

#include "parser/lexer.hpp"

#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
  EvaVm vm;
  auto result = vm.exec(R"(

  )");

  log(AS_CPPSTRING(result));

  std::cout << "Porgram executed successfully\n";

  Lexer lexer = Lexer("123");
  auto tokens = lexer.tokenize();

  for (auto i : tokens) {
    std::cout << i.type_to_string() << " : " << i.literal << "\n";
  }
  return 0;
}
