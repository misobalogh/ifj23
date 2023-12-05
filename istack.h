#ifndef I_STACK_H
#define I_STACK_H

typedef struct IStack {
  unsigned* data;
  unsigned size;
  unsigned capacity;
} IStack;

IStack* istackInit(void);
void istackFree(IStack* stack);
unsigned istackPop(IStack* stack);
void istackPush(IStack* stack, unsigned value);
unsigned istackTop(IStack* stack);

#endif
