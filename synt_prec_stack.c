/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_prec_stack.c
* Description: Stack for state machine in syntactic analysis
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 13.10.2023

* Comments:

***************************************************************/

#include "synt_prec_stack.h"
#include "token_types.h"
#include <stdlib.h>
#include <string.h>


/**
 * @brief Initialize stack.
 *
 * @param s Pointer to stack.
 */
void stackInit(stack* s) {
    s->size = 0;
    s->top = NULL;
}

/**
 * @brief Push new item on stack.
 *
 * @param s Pointer to stack.
 * @param type Type of token.
 */
void stackPush(stack* s, lex_token token) {
    stackItem* newItem = malloc(sizeof(stackItem));
    CHECK_MEMORY_ALLOC(newItem);
    newItem->token = token;

    /* if (token.type == token_ID) { */
    /*   size_t len = strlen(token.value.STR_VAL) + 1; */
    /*   newItem->token.value.STR_VAL = malloc(len); */
    /*   CHECK_MEMORY_ALLOC(newItem->token.value.STR_VAL); */
    /*   strncpy(newItem->token.value.STR_VAL, token.value.STR_VAL, len); */
    /* } */

    newItem->lower = s->top;
    newItem->flag = false;
    s->top = newItem;
    s->size++;
}

/**
 * @brief Pop item from stack.
 * If stack is empty, nothing happens.
 * Also frees memory of popped item.
 *
 * @param s Pointer to stack.
*/
void stackPop(stack* s) {
    if (stackIsEmpty(s)) {
        return;
    }
    stackItem* topItem = s->top;

    if (topItem->token.type == token_ID) {
      free(topItem->token.value.STR_VAL);
    }

    s->top = s->top->lower;
    s->size--;
    free(topItem);
}

/**
 * @brief Returns top of stack.
 *
 * @param s Pointer to stack.
 * @return stackItem* pointer to item on top of stack.
 * NULL if stack is empty.
 */
stackItem* stackTop(stack* s) {
    return s->top;
}

/**
 * @brief Returns true if stack is empty.
 *
 * @param s Pointer to stack.
*/
bool stackIsEmpty(stack* s) {
    return s->size == 0;
}

/**
 * @brief Frees all items on stack.
 *
 * @param s Pointer to stack.
*/
void stackFreeItems(stack* s) {
    while (!stackIsEmpty(s)) {
        stackPop(s);
    }
}


/**
 * @brief Returns first terminal from stack.
 *
 * @param s Pointer to stack.
 * @return stackItem* pointer to first terminal on stack.
 * NULL if stack is empty or there is no terminal on stack.
*/
stackItem* stackTopTerminal(stack* s) {
    stackItem* item = s->top;
    while (item && !isTerminal(item->token.type)) {
        item = item->lower;
    }
    return item;
}

/**
 * @brief Sets flag of first terminal on stack.
 * means: change 'a' to 'a<'
 *
 * @param s Pointer to stack.
 */
void stackTopTerminalSetFlag(stack* s) {
    stackItem* item = s->top;
    while (item && !isTerminal(item->token.type)) {
        item = item->lower;
    }
    item->flag = true;
}

/**
 * @brief Returns first item from stack. (top)
 *
 * @param s Pointer to stack.
 *
 * @return stackItem* pointer to first item on stack.
 * NULL if stack is empty.
*/
stackItem* stackFirst(stack* s) {
    return s->top;
}

/**
 * @brief Returns second item from stack. (top - 1)
 *
 * @param s Pointer to stack.
 *
 * @return stackItem* pointer to second item on stack.
 * NULL if stack is empty or there is no second item on stack.
*/
stackItem* stackSecond(stack* s) {
    if (s->size < 2) {
        return NULL;
    }
    return s->top->lower;
}

/**
 * @brief Returns third item from stack. (top - 2)
 *
 * @param s Pointer to stack.
 *
 * @return stackItem* pointer to third item on stack.
 * NULL if stack is empty or there is no third item on stack.
*/
stackItem* stackThird(stack* s) {
    if (s->size < 3) {
        return NULL;
    }
    return s->top->lower->lower;
}


//========== DEBUG ==========
/**
 * @brief Prints stack.
 *
 * @param s Pointer to stack.
*/
void stackPrint(stack* s) {
    FILE* logFile = fopen("precedenceParser.log", "a");
    if (!logFile) {
        return;
    }
    stackItem* item = s->top;
    while (item) {
        const char* str = TokenName(item->token.type);
        fprintf(logFile, "%s\n", str);
        item = item->lower;
    }
    fprintf(logFile, "\n\n");
    fclose(logFile);
}
