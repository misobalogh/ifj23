/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_precedence_parser.c
* Description: Precedence syntactic analysis (parser for expression)
* Author: MICHAL BALOGH, xbalog06, MICHAL CENEK xcenek04
* Faculty: FIT VUT
* Date: 14.10.2023

* Comments:

***************************************************************/

#include "synt_analysis.h"
#include "synt_prec_stack.h"
#include "synt_prec_table.h"
#include "token_types.h"
#include "synt_prec_rules.h"


/**
 * @brief Gives information if current token is some type of constant
 *
 * @return true if token is constant, false otherwise
*/
bool connstTypes(tokenType type) {
    if (type == token_CONST
        || type == token_CONST_WHOLE_NUMBER
        || type == token_CONST_DEC_NUMBER
        || type == token_CONST_SCIENTIFIC_NOTATION
        || type == token_TYPE_STRING_LINE
        || type == token_NIL) {
        return true;
    }
    return false;
}

/**
 * @brief Gives information if current token is valid part of expression
 *
 * @return true if token can be part of expression, false otherwise
*/
bool possibleExpressionTokensWithoutID() {
    if (connstTypes(t.type)
        || t.type == token_PARENTHESES_L
        || t.type == token_PARENTHESES_R
        || (t.type >= token_OP_START && t.type <= token_OP_END)) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Get index for precedence table
 *
 * @param token to get index for
 *
 * @return index to the table
*/
int getTableIndex(tokenType token) {
    switch (token)
    {
    case token_PLUS:
    case token_MINUS:
        return 0;
    case token_MUL:
    case token_DIV:
        return 1;
    case token_DEFAULT_VALUE:
        return 2;
    case token_FORCE_UNWRAP:
        return 3;
    case token_REL:
    case token_EQ:
    case token_NEQ:
    case token_LESS:
    case token_MORE:
    case token_LESS_EQ:
    case token_MORE_EQ:
        return 4;
    case token_PARENTHESES_L:
        return 5;
    case token_PARENTHESES_R:
        return 6;
    case token_ID:
    case token_CONST:
    case token_NIL:
    case token_CONST_WHOLE_NUMBER:
    case token_CONST_DEC_NUMBER:
    case token_CONST_SCIENTIFIC_NOTATION:
    case token_TYPE_STRING_LINE:
        return 7;
    case token_DOLLAR:
    case token_EOL:
    case token_BRACKET_L:
    case token_BRACKET_R:
    case token_EOF:
        return 8;
    default:
        return 8;
    }
}

#include "semantic_analysis.h"


/**
 * @brief Reduce items on stack with one of defined rules.
 * Called when ">" sign is in precedence table.
 * It gets top three items from stack and calls rule function, and then chooses,
 * which rule should be applied, according to types of items.
 *
 * @param s Pointer to stack.
 *
 * @return True if rule was applied, false otherwise if rule does not exist
*/
bool reduce(stack* s) {
    stackItem* top = stackTopTerminal(s);
    stackItem* first = stackFirst(s);
    stackItem* second = stackSecond(s);
    stackItem* third = stackThird(s);

    // rule ID
    if ((top->token.type == token_ID
        || top->token.type == token_CONST
        || top->token.type == token_CONST_WHOLE_NUMBER
        || top->token.type == token_CONST_DEC_NUMBER
        || top->token.type == token_CONST_SCIENTIFIC_NOTATION
        || top->token.type == token_TYPE_STRING_LINE
        || top->token.type == token_NIL) && top->flag == true) {
        // PLOG("---rule ID---");

        analyseExprOperand(top->token);

        rule_E_ID(s);
        // stackPrint(s);
    }
    // rule E op E
    else if (
        first->token.type == token_NONTERMINAL &&
        (second->token.type == token_PLUS || second->token.type == token_MINUS || second->token.type == token_MUL || second->token.type == token_DIV) &&
        third->token.type == token_NONTERMINAL) {
        // PLOG("---rule E op E---");

        analyseExprOperator(second->token);

        rule_ID_OP_ID(s);
        // stackPrint(s);
    }
    // rule E ?? E
    else if (
        first->token.type == token_NONTERMINAL &&
        second->token.type == token_DEFAULT_VALUE &&
        third->token.type == token_NONTERMINAL) {
        // PLOG("---rule E ?? E---");

        analyseExprDefault();

        rule_ID_CONCAT_ID(s);
        // stackPrint(s);
    }
    // rule (E)
    else if (
        first->token.type == token_PARENTHESES_R &&
        second->token.type == token_NONTERMINAL &&
        third->token.type == token_PARENTHESES_L) {
        // PLOG("---rule (E)---");
        rule_PAR_ID_PAR(s);
        // stackPrint(s);
    }
    // rule E!
    else if (
        first->token.type == token_FORCE_UNWRAP &&
        second->token.type == token_NONTERMINAL) {
        // PLOG("---rule E!---");

        analyseExprOperator(first->token);

        rule_ID_FORCE_UNWRAP(s);
        // stackPrint(s);
    }
    // rule ID rel ID
    else if (
        first->token.type == token_NONTERMINAL
        && (second->token.type == token_REL
            || second->token.type == token_EQ
            || second->token.type == token_NEQ
            || second->token.type == token_LESS
            || second->token.type == token_MORE
            || second->token.type == token_LESS_EQ
            || second->token.type == token_MORE_EQ)
        && third->token.type == token_NONTERMINAL) {
        // PLOG("rule ID rel ID\n");

        analyseExprOperator(second->token);

        // stackPrint(s);
        rule_ID_REL_ID(s);
    }
    else {
        // PLOG("ERROR: unknown rule\n");
        return false;
    }

    return true;
}



bool precedenceParser() {
    // prepare stack
    stack s;
    stackInit(&s);
    lex_token tokenDollar = { .type = token_DOLLAR, .value = 0 };
    stackPush(&s, tokenDollar);

    // PLOG("====INIT====");
    // stackPrint(&s);

    tokenType lastToken = token_DOLLAR;

    // swap stash and current token
    if (stash.type != token_EMPTY) {
        lex_token temp = t;
        t = stash;
        stash = temp;
    }

    // flag to check if we even entered the while loop, if not -> not valid tokens for expressions were found -> return false
    bool expression_is_empty = true;

    while (possibleExpressionTokensWithoutID() || t.type == token_ID || stackTopTerminal(&s)->token.type != token_DOLLAR) {
        // LOG("Last token: %s", TokenName(lastToken));
        // LOG("Current token: %s\n", TokenName(t.type));

        int table_index2;

        if ((lastToken == token_ID
            || connstTypes(lastToken)
            || lastToken == token_PARENTHESES_R
            || lastToken == token_FORCE_UNWRAP) && t.type == token_ID) {
            table_index2 = getTableIndex(token_DOLLAR);
        }
        else {
            table_index2 = getTableIndex(t.type);
        }

        int table_index1 = getTableIndex(stackTopTerminal(&s)->token.type);

        if (table_index1 == getTableIndex(token_DOLLAR) && table_index2 == getTableIndex(token_DOLLAR)) {
            break;
        }

        // LOG("table indexes: %d %d\n", table_index1, table_index2);
        expression_is_empty = false;
        switch (precedenceTable[table_index1][table_index2])
        {
        case EQUAL: // "="
            stackPush(&s, t);

            lastToken = t.type;
            getToken();

            // PLOG("====EQUAL====");
            // stackPrint(&s);
            break;
        case LOW: // expand  "<"
            stackPush(&s, t);
            stackTopTerminalSetFlag(&s);

            lastToken = t.type;
            getToken();

            // PLOG("====LOW====");
            // stackPrint(&s);
            break;
        case HIGH: // reduce ">"
            if (reduce(&s)) {
                // PLOG("====HIGH====");
                // stackPrint(&s);
                break;
            }
            else {
                stackFreeItems(&s);
                return false;
            }
        case EMPTY: // error
            // PLOG("====EMPTY====");
            // stackPrint(&s);
            stackFreeItems(&s);
            return false;
        default:
            // PLOG("ERROR: unknown precedence table value\n");
            return false;
        }
    }

    stash.type = token_EMPTY;

    // PLOG("====END====\n");
    stackFreeItems(&s);

    if (expression_is_empty) {
        return false;
    }

    return true;
}

