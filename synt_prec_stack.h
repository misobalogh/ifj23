/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_prec_stack.h
* Description: Function declarations for stack for state machine in syntactic analysis
* Author: MICHAL BALOGH, xbalog06
* Faculty: FIT VUT
* Date: 13.10.2023

* Comments: 

***************************************************************/

#ifndef IFJ23_SYNT_PREC_STACK_H_
#define IFJ23_SYNT_PREC_STACK_H_

#include <stdbool.h>
#include "token_types.h"

/**
 * @brief Item of stack.
 * 
 * @param type Type of token.
 * @param flag Flag wheter "<" sign from precedence table is set.
 * @param lower Pointer to item below this item.
*/
typedef struct stackItem {
    tokenType type;
    bool flag; 
    struct stackItem *lower;
} stackItem;

/**
 * @brief Stack struct for state machine in syntactic analysis.
 * 
 * @param top Pointer to top item of stack.
 * @param size Size of stack.
*/
typedef struct stack {
    stackItem* top;
    unsigned size;
} stack;

void stackInit(stack *s);

void stackPush(stack *s, tokenType type);

void stackPop(stack *s);

stackItem* stackTop(stack *s);

bool stackIsEmpty(stack *s);

void stackFreeItems(stack *s);

stackItem* stackTopTerminal(stack *s);

void stackTopTerminalSetFlag(stack* s);

stackItem* stackFirst(stack *s);

stackItem* stackSecond(stack *s);

stackItem* stackThird(stack *s);

//========= DEBUG =========
void stackPrint(stack *s);  


#endif // IFJ23_SYNT_PREC_STACK_H_