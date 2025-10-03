#ifndef EVA_VALUE
#define EVA_VALUE

#include <string>
enum class EvaValueType {
  NUMBER,
  OBJECT,
};

enum class ObjectType {
  STRING,
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
    Object *object;
  };
};

// Constructors
#define NUMBER(value)                                                          \
  ((EvaValue)(EvaValue{EvaValueType::NUMBER, {.number = value}}))

#define ALLOC_STRING(value)                                                    \
  ((EvaValue){EvaValueType::OBJECT,                                            \
              {.object = (Object *)new StringObject(value)}})

// Checkers
#define IS_NUMBER(evaValue) ((evaValue).type == EvaValueType::NUMBER)
#define IS_OBJECT(evaValue) ((evaValue).type == EvaValueType::OBJECT)

#define IS_OBJECT_TYPE(evaValue, ObjectType)                                   \
  (IS_OBJECT(evaValue) && AS_OBJECT(evaValue)->type == ObjectType)

#define IS_STRING(evaValue) (IS_OBJECT_TYPE(evaValue, ObjectType::STRING))

// Accessors
#define AS_NUMBER(evaValue) ((double)(evaValue).number)
#define AS_OBJECT(evaValue) ((Object *)(evaValue).object)

#define AS_STRING(evaValue) ((StringObject *)(evaValue).object)
#define AS_CPPSTRING(evaValue) (AS_STRING(evaValue)->string)

#endif // !EVA_VALUE
