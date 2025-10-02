#ifndef EVA_VALUE
#define EVA_VALUE

enum class EvaValueType {
  NUMBER,
};

struct EvaValue {
  EvaValueType type;
  union {
    double number;
  };
};

#define NUMBER(value)                                                          \
  ((EvaValue)(EvaValue{EvaValueType::NUMBER, {.number = value}}))

#define AS_NUMBER(evaValue) ((double)(evaValue).number)

#endif // !EVA_VALUE
