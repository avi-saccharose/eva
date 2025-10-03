#ifndef EVA_VALUE
#define EVA_VALUE

#include <cstdint>
#include <string>
#include <vector>

enum class EvaValueType {
  NUMBER,
  BOOLEAN,
  OBJECT,
};

enum class ObjectType {
  STRING,
  CODE,
};

struct Object {
  Object(ObjectType type) : type(type) {}
  ObjectType type;
};

struct StringObject : Object {
  StringObject(const std::string &string)
      : Object(ObjectType::STRING), string(string) {}
  std::string string;
};

struct EvaValue {
  EvaValueType type;
  union {
    double number;
    bool boolean;
    Object *object;
  };
};

struct CodeObject : Object {
  CodeObject(const std::string &name) : Object(ObjectType::CODE), name(name) {}
  std::string name;
  std::vector<EvaValue> constants;
  std::vector<uint8_t> code;
};

// Constructors
#define NUMBER(value) \
  ((EvaValue)(EvaValue{EvaValueType::NUMBER, {.number = value}}))

#define BOOLEAN(value) \
  ((EvaValue)(EvaValue{EvaValueType::BOOLEAN, {.boolean = value}}))

#define ALLOC_STRING(value)         \
  ((EvaValue){EvaValueType::OBJECT, \
              {.object = (Object *)new StringObject(value)}})

#define ALLOC_CODE(value)           \
  ((EvaValue){EvaValueType::OBJECT, \
              {.object = (Object *)new CodeObject(value)}})

// Checkers
#define IS_NUMBER(evaValue) ((evaValue).type == EvaValueType::NUMBER)
#define IS_BOOLEAN(evaValue) ((evaValue).type == EvaValueType::BOOLEAN)
#define IS_OBJECT(evaValue) ((evaValue).type == EvaValueType::OBJECT)

#define IS_OBJECT_TYPE(evaValue, ObjectType) \
  (IS_OBJECT(evaValue) && AS_OBJECT(evaValue)->type == ObjectType)

#define IS_STRING(evaValue) (IS_OBJECT_TYPE(evaValue, ObjectType::STRING))
#define IS_CODE(evaValue) (IS_OBJECT_TYPE(evaValue, ObjectType::CODE))

// Accessors
#define AS_NUMBER(evaValue) ((double)(evaValue).number)
#define AS_BOOLEAN(evaValue) ((bool)(evaValue).boolean)
#define AS_OBJECT(evaValue) ((Object *)(evaValue).object)

#define AS_STRING(evaValue) ((StringObject *)(evaValue).object)
#define AS_CPPSTRING(evaValue) (AS_STRING(evaValue)->string)

#define AS_CODE(evaValue) ((CodeObject *)(evaValue).object)

#endif  // !EVA_VALUE
