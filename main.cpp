#include "logger.hpp"
#include "vm/eva_value.hpp"
#include "vm/eva_vm.hpp"

#include "parser/ast_printer.hpp"
#include "parser/parser.hpp"

#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
  EvaVm vm;
  auto result = vm.exec(R"(

  )");

  log(AS_CPPSTRING(result));

  std::cout << "Program executed successfully\n";

  Parser parser = Parser();
  AstPrinter printer = AstPrinter();

  auto exprs = parser.parse("123 + 3");

  for (auto expr : exprs) {
    std::cout << printer.print(expr) << std::endl;
    delete expr;
  }

  return 0;
}
