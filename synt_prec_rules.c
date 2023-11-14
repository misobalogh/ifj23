/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: rules.c
* Description: rules for syntactic analysis
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 15.10.2023

* Comments:

***************************************************************/


#include "synt_prec_rules.h"
#include "synt_analysis.h"

const precedenceType precedenceTable[10][10] = {
    //+-----+-----+------+------+------+-------+-------+------+-------+-------+------+
    //      |  +- |  */  |  ??  |   !  |  REL  |   (   |   )  |   i   | CONST |   $  |
    //+-----+-----+------+------+------+-------+-------+------+-------+-------+------+---+------
            {HIGH,  LOW,   HIGH,  LOW,   HIGH,   LOW,    HIGH,   LOW,   LOW,   HIGH},//  |  +-  |
            {HIGH,  HIGH,  HIGH,  LOW,   HIGH,   LOW,    HIGH,   LOW,   LOW,   HIGH},//   |  */  |
            {LOW,   LOW,   LOW,   LOW,   LOW,    LOW,    HIGH,   LOW,   LOW,   HIGH},//   |  ??  |
            {HIGH,  HIGH,  HIGH,  EMPTY, HIGH,   EMPTY,  HIGH,   EMPTY, EMPTY, HIGH},//|  !   |
            {LOW,   LOW,   HIGH,  LOW,   EMPTY,  LOW,    HIGH,   LOW,   LOW,   HIGH},//   |  REL |
            {LOW,   LOW,   LOW,   LOW,   LOW,    LOW,    EQUAL,  LOW,   LOW,   EMPTY},//  |  (   |
            {HIGH,  HIGH,  HIGH,  HIGH,  HIGH,   EMPTY,  HIGH,   EMPTY, EMPTY, HIGH},// |  )   |
            {HIGH,  HIGH,  HIGH,  HIGH,  HIGH,   EMPTY,  HIGH,   EMPTY, EMPTY, HIGH},// |  i   |
            {HIGH,  HIGH,  HIGH,  HIGH,  HIGH,   EMPTY,  HIGH,   EMPTY, EMPTY, HIGH},// | CONST|
            {LOW,   LOW,   LOW,   LOW,   LOW,    LOW,    EMPTY,  LOW,   LOW,   EMPTY}//   |  $   |
};

/**
 * @brief Rule: E -> id
*/
void rule_E_ID(stack* s) {
    stackItem* top = stackTopTerminal(s);
    top->flag = false;
    top->type = token_NONTERMINAL;
}

/**
 * @brief Rule: E -> E arithmetic_operator E
 * Rule: E -> E + E
 * Rule: E -> E - E
 * Rule: E -> E * E
 * Rule: E -> E / E
*/
void rule_ID_OP_ID(stack* s) {
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}

/**
 * @brief Rule: E -> (E)
*/
void rule_PAR_ID_PAR(stack* s) {
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}

/**
 * @brief Rule: E -> E ?? E
 */
void rule_ID_CONCAT_ID(stack* s) {
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}


/**
 * @brief Rule: E -> E!
 */
void rule_ID_FORCE_UNWRAP(stack* s) {
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}


void rule_ID_REL_ID(stack* s) {
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}