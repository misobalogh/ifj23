#include "rules.h"

// const precedenceType precedenceTable[6][6] = {
// //+-----+-----+-----+-------+-------+-------+-------+
// //      |  +  |  *  |   (   |   )   |   id  |   $   |
// //+-----+-----+-----+-------+-------+-------+-------+
// /* + */ { HIGH, LOW,   LOW,    HIGH,   LOW,   HIGH  },
// /* * */ { HIGH, HIGH,  LOW,    HIGH,   LOW,   HIGH  },
// /* ( */ { LOW,  LOW,   LOW,    EQUAL,  LOW,   EMPTY },
// /* ) */ { HIGH, HIGH,  EMPTY,  HIGH,   EMPTY, HIGH  },
// /* id */{ HIGH, HIGH,  EMPTY,  HIGH,   EMPTY, HIGH  },
// /* $  */{ LOW,  LOW,   LOW,    EMPTY,  LOW,   EMPTY },
// //+-----+-----+-----+-------+-------+-------+------ -+
// };


const precedenceType precedenceTable[9][9] = {
//+-----+-----+------+------+------+-------+-------+------+-------+------+
//      |  +- |  */  |  ??  |   !  |  REL  |   (   |   )  |   i   |   $  |
//+-----+-----+------+------+------+-------+-------+------+-------+------+---+------
        {HIGH,  LOW,   HIGH,  LOW,   HIGH,   LOW,    HIGH,   LOW,   HIGH},// |  +-  |
        {HIGH,  HIGH,  HIGH,  LOW,   HIGH,   LOW,    HIGH,   LOW,   HIGH},// |  */  |
        {LOW,   LOW,   LOW,   LOW,   LOW,    LOW,    HIGH,   LOW,   HIGH},// |  ??  |
        {HIGH,  HIGH,  HIGH,  EMPTY, HIGH,   EMPTY,  HIGH,   EMPTY, HIGH},// |  !   |
        {LOW,   LOW,   HIGH,  LOW,   EMPTY,  LOW,    HIGH,   LOW,   HIGH},// |  REL |
        {LOW,   LOW,   LOW,   LOW,   LOW,    LOW,    EQUAL,  LOW,   EMPTY},//|  (   |
        {HIGH,  HIGH,  HIGH,  HIGH,  HIGH,   EMPTY,  HIGH,   EMPTY, HIGH},// |  )   |
        {HIGH,  HIGH,  HIGH,  HIGH,  HIGH,   EMPTY,  HIGH,   EMPTY, HIGH},// |  i   |
        {LOW,   LOW,   LOW,   LOW,   LOW,    LOW,    EMPTY,  LOW,   EMPTY}// |  $   |
};

/**
 * @brief Rule: E -> id
*/
void rule_ID(stack* s) {
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


void rule_ID_REL_ID(stack* s){
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}