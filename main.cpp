#include "eva_value.hpp"
#include "eva_vm.hpp"
#include "logger.hpp"

#include <iostream>

int main(int argc, char const *argv[]) {
  EvaVm vm;
  auto result = vm.exec(R"(

  )");

  log(AS_NUMBER(result));

  std::cout << "Porgram executed successfully\n";
  return 0;
}
