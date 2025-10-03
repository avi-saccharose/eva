#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdio>
#include <cstdlib>
#include <sstream>

class ErrorLogMessage : public std::basic_ostringstream<char> {
 public:
  ~ErrorLogMessage() {
    fprintf(stderr, "fatal error: %s\n", str().c_str());
    exit(EXIT_FAILURE);
  }
};

#define DIE ErrorLogMessage()

#define log(value) std::cout << #value << " = " << (value) << "\n";

#endif
