#include "eva_value.hpp"

#include <sstream>

#include "../logger.hpp"

// Output stream
std::string evaValueToTypeString(const EvaValue &value) {
  if (IS_NUMBER(value)) {
    return "NUMBER";
  } else if (IS_BOOLEAN(value)) {
    return "BOOLEAN";
  } else if (IS_STRING(value)) {
    return "STRING";
  } else if (IS_CODE(value)) {
    return "CODE";
  } else {
    DIE << "evaValueToTypeString: unknown type" << (int)value.type;
  }
  return "";
}

std::string evaValueToConstantString(const EvaValue &value) {
  std::stringstream ss;
  if (IS_NUMBER(value)) {
    ss << value.number;
  } else if (IS_BOOLEAN(value)) {
    ss << (value.boolean ? "true" : "false");
  } else if (IS_STRING(value)) {
    ss << '"' << AS_CPPSTRING(value) << '"';
  } else if (IS_CODE(value)) {
    auto code = AS_CODE(value);
    ss << "code " << code << ": " << code->name;
  } else {
    DIE << "evaValueToConstantString: unknown type" << (int)value.type;
  }
  return ss.str();
}

std::ostream &operator<<(std::ostream &os, const EvaValue &value) {
  return os << "EvaValue (" << evaValueToTypeString(value)
            << "): " << evaValueToConstantString(value);
}
