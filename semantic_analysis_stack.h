#ifndef SEMANTIC_ANALYSIS_STACK_H
#define SEMANTIC_ANALYSIS_STACK_H

#include <stdint.h>
#include <stdbool.h>
#include "swift_keywords.h"
#include "token_types.h"

typedef enum { SEM_STACK_TOKEN, SEM_STACK_LITERAL } semStackValueType;

typedef struct SemStackValue {
  semStackValueType valueType;
  union {
    enum swift_types swift_type;
    tokenType token_type;
  };
  union {
    char* key;
    int64_t intValue;
    double floatValue;
  };
} SemStackValue;

typedef struct SemStackElement {
  SemStackValue value;
  struct SemStackElement* next;
} SemStackElement;

typedef struct SemStack {
  SemStackElement* top;
  size_t size;
} SemStack;

void semStackInit(SemStack* stack);

bool semStackPush(SemStack* stack, SemStackValue element);

bool semStackPushToken(SemStack* stack, struct token token);
void semStackPushInt(SemStack* stack, int32_t value);
void semStackPushDouble(SemStack* stack, double value);

void semStackPop(SemStack* stack);

SemStackElement* semStackTop(SemStack* stack);

SemStackElement* semStackPopTop(SemStack* stack);

bool semStackIsEmpty(SemStack* stack);

void semStackFreeItems(SemStack* stack);


#endif
