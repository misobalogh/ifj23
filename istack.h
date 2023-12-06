/*
 * Project: Implementace překladače imperativního jazyka IFJ23
 * Authors: Michal Cenek xcenek04
*/

#ifndef I_STACK_H
#define I_STACK_H


// stack of integers
typedef struct IStack {
  unsigned* data;
  unsigned size;
  unsigned capacity;
} IStack;

// initialize stack
IStack* istackInit(void);

// free stack memory
void istackFree(IStack* stack);

// remove value at top fo stack and return its value
unsigned istackPop(IStack* stack);

// add value on top of stack
void istackPush(IStack* stack, unsigned value);

// return value on top of stack
unsigned istackTop(IStack* stack);

#endif
