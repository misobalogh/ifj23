/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_recur_rules.c
* Description: rules for top-down syntactic analysis
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 21.10.2023

* Comments:

***************************************************************/

#include "synt_analysis.h"
#include "macros.h"
#include "synt_recur_rules.h"


/**
 * @brief Stash for token for expression parser
 * when switching from recursive parser when deciding,
 * whether it is expression or function call
 */
tokenStruct* stash;


/**
 * @brief Most recent token from scanner
 */
tokenStruct* t;

bool rule_EXPRESSION() {
    RLOG("<expression> => switching to precedence parser\n");
    return precedenceParser();
}

bool rule_PROGRAM() {
    // 1. <program> -> <stat_list> EOF
    t = mock_recursive_nextToken();
    RLOG("<program> -> <stat_list> EOF\n");
    if (t->type == token_EOL ||
        t->type == token_LET ||
        t->type == token_VAR ||
        t->type == token_IF ||
        t->type == token_WHILE ||
        t->type == token_ID ||
        t->type == token_FUNC ||
        t->type == token_EOF) {
        if (rule_STAT_LIST()) {
            RLOG("EOF\n");
            return true;
        }
    }
    return false;
}

bool rule_STAT_LIST() {
    // 2. <stat_list> -> <statement> EOL <stat_list>
    if (t->type == token_LET ||
        t->type == token_VAR ||
        t->type == token_IF ||
        t->type == token_WHILE ||
        t->type == token_ID ||
        t->type == token_FUNC ||
        t->type == token_EOL) {
        RLOG("<stat_list> -> <statement> EOL <stat_list>\n");
        if (rule_STATEMENT()) {
            if (t->type == token_EOL) {
                t = mock_recursive_nextToken();
                return rule_STAT_LIST();
            }
        }
    }
    // 3. <stat_list> -> EPSILON
    else if (t->type == token_EOF) {
        t = mock_recursive_nextToken();
        RLOG("<stat_list> -> EPSILON\n");
        return true;
    }
    return false;
}


bool rule_BRACK_STAT_LIST() {
    // 2. <brack_stat_list> -> <brack_statement> EOL <between_brackets>
    if (t->type == token_LET ||
        t->type == token_VAR ||
        t->type == token_IF ||
        t->type == token_WHILE ||
        t->type == token_ID ||
        t->type == token_FUNC || t->type == token_EOL

        ) {
        RLOG("<brack_stat_list> -> <statement> EOL <brack_stat_list>\n");
        if (rule_STATEMENT()) {
            if (t->type == token_EOL) {
                t = mock_recursive_nextToken();
                return rule_BRACK_STAT_LIST();
            }
        }
    }
    if (t->type == token_BRACKET_R) {
        RLOG("<brack_stat_list> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_STATEMENT() {
    switch (t->type) {
    case token_LET:
    case token_VAR:
        RLOG("<statement> -> <let_or_var> <var_assignment>\n");
        return rule_LET_OR_VAR() && rule_VAR_ASSIGNMENT();
    case token_ID:
        RLOG("<statement> -> id <after_id>\n");
        t = mock_recursive_nextToken();
        return rule_AFTER_ID();
    case token_FUNC:
        RLOG("<statement> -> func id ( <param_list> ) <return_type> { <func_stat_list> }\n");
        t = mock_recursive_nextToken();
        if (t->type != token_ID) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_PARENTHESES_L) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (!rule_PARAM_LIST()) {
            return false;
        }
        if (t->type != token_PARENTHESES_R) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (!rule_RETURN_TYPE()) {
            return false;
        }
        if (t->type != token_BRACKET_L) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (!rule_FUNC_STAT_LIST()) {
            return false;
        }
        if (t->type != token_BRACKET_R) {
            return false;
        }
        t = mock_recursive_nextToken();
        return true;
    case token_IF:
        RLOG("<statement> -> if <condition> { <brack_stat_list> } else { <brack_stat_list> }\n");
        t = mock_recursive_nextToken();
        if (!rule_CONDITION()) {
            return false;
        }
        if (t->type != token_BRACKET_L) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        if (t->type != token_BRACKET_R) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_ELSE) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_BRACKET_L) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        if (t->type != token_BRACKET_R) {
            return false;
        }
        t = mock_recursive_nextToken();
        return true;
    case token_WHILE:
        RLOG("<statement> -> while <expression> { <brack_stat_list> }\n");
        t = mock_recursive_nextToken();
        if (!rule_EXPRESSION()) {
            return false;
        }
        if (t->type != token_BRACKET_L) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        if (t->type != token_BRACKET_R) {
            return false;
        }
        t = mock_recursive_nextToken();
        return true;
    case token_EOL:
        RLOG("<statement> -> EPSILON\n");
        return true;
    default:
        RLOG("ERROR: <statement>\n");
        return false;
    }
}

bool rule_LET_OR_VAR() {
    if (t->type == token_LET) {
        RLOG("<let_or_var> -> let id\n");
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    else if (t->type == token_VAR) {
        RLOG("<let_or_var> -> var id\n");
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    return false;
}

bool rule_VAR_ASSIGNMENT() {
    // <var_assigment> -> : type <val_assigment>
    if (t->type == token_COLON) {
        RLOG("<var_assignment> -> : type <val_assignment>\n");
        t = mock_recursive_nextToken();
        if (t->type == token_TYPE) {
            t = mock_recursive_nextToken();
            return rule_VAL_ASSIGNMENT();
        }
    }
    // <var_assigment> -> = id <fn_or_exp>
    else if (t->type == token_ASSIGN) {
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
        RLOG("<var_assignment> -> = id <fn_or_exp>\n");
            stash = t;
            t = mock_recursive_nextToken();
            return rule_FN_OR_EXP();
        }
        else if (t->type == token_CONST) {
        RLOG("<var_assignment> -> = const <expression>\n");
            return rule_EXPRESSION();
        }
    }
    return false;
}

bool rule_VAL_ASSIGNMENT() {
    // <val_assigment> -> = id <fn_or_exp>
    if (t->type == token_ASSIGN) {
        RLOG("<val_assignment> -> = id <fn_or_exp>\n");
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            stash = t;
            t = mock_recursive_nextToken();
            return rule_FN_OR_EXP();
        }
        else if (t->type == token_CONST) {
        RLOG("<val_assignment> -> = const <expression>\n");
            return rule_EXPRESSION();
        }
    }
    // <val_assigment> -> EPSILON
    else if (t->type == token_EOL) {
        RLOG("<val_assignment> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_FN_OR_EXP() {
    // <fn_or_exp> -> EOL
    if (t->type == token_EOL) {
        return true;
    }
    // <fn_or_exp> -> <expression> 
    if ((t->type >= token_OP_START && t->type <= token_OP_END)) { // TODO: is it correct for every case?
        RLOG("<fn_or_exp> -> <expression>\n");
        return rule_EXPRESSION();
    }
    // <fn_or_exp> -> ( <input_param_list> )
    else if (t->type == token_PARENTHESES_L) {
        RLOG("<fn_or_exp> -> ( <input_param_list> )\n");
        t = mock_recursive_nextToken();
        if (!rule_INPUT_PARAM_LIST()) {
            return false;
        }
        if (t->type != token_PARENTHESES_R) {
            return false;
        }
        t = mock_recursive_nextToken();
        return true;
    }
    return false;
}

bool rule_AFTER_ID() {
    // <after_id> -> = id <fn_or_exp> 
    if (t->type == token_ASSIGN) {
        RLOG("<after_id> -> = id <fn_or_exp>\n");
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            stash = t;
            t = mock_recursive_nextToken();
            return rule_FN_OR_EXP();
        }
    }
    // <after_id> -> ( <input_param_list> ) 
    else if (t->type == token_PARENTHESES_L) {
        RLOG("<after_id> -> ( <input_param_list> )\n");
        t = mock_recursive_nextToken();
        if (!rule_INPUT_PARAM_LIST()) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_PARENTHESES_R) {
            return false;
        }
        t = mock_recursive_nextToken();
        return true;
    }
    return false;
}

bool rule_INPUT_PARAM_LIST() {
    // <input_param_list> -> EPSILON
    if (t->type == token_PARENTHESES_R) {
        RLOG("<input_param_list> -> EPSILON\n");
        return true;
    }
    // <input_param_list> -> <input_param> <input_param_next>
    else if (t->type == token_ID || t->type == token_CONST) {
        RLOG("<input_param_list> -> <input_param> <input_param_next>\n");
        return rule_INPUT_PARAM() && rule_INPUT_PARAM_NEXT();
    }
    return false;
}

bool rule_INPUT_PARAM_NEXT() {
    // <input_param_next> -> , <input_param> <input_param_next>
    if (t->type == token_COMMA) {
        t = mock_recursive_nextToken();
        return rule_INPUT_PARAM() && rule_INPUT_PARAM_NEXT();
    }
    // <input_param_next> -> EPSILON
    else if (t->type == token_PARENTHESES_R) {
        return true;
    }
    return false;
}

bool rule_INPUT_PARAM() {
    // <input_param> -> const        
    if (t->type == token_CONST) {
        RLOG("<input_param> -> const\n");
        t = mock_recursive_nextToken();
        return true;
    }
    // <input_param> -> id <with_name>
    else if (t->type == token_ID) {
        RLOG("<input_param> -> id <with_name>\n");
        t = mock_recursive_nextToken();
        return rule_WITH_NAME();
    }
    return false;
}

bool rule_WITH_NAME() {
    // <with_name> -> EPSILON 
    if (t->type == token_COMMA ||
        t->type == token_PARENTHESES_R) {
        RLOG("<with_name> -> EPSILON\n");
        return true;
    }
    // <with_name> -> : <id_or_const>
    else if (t->type == token_COLON) {
        RLOG("<with_name> -> : <id_or_const>\n");
        t = mock_recursive_nextToken();
        return rule_ID_OR_CONST();
    }
    return false;
}

bool rule_ID_OR_CONST() {
    // <id_or_const> -> id
    if (t->type == token_ID) {
        RLOG("<id_or_const> -> id\n");
        t = mock_recursive_nextToken();
        return true;
    }
    // <id_or_const> -> const
    else if (t->type == token_CONST) {
        RLOG("<id_or_const> -> const\n");
        t = mock_recursive_nextToken();
        return true;
    }
    return false;
}

bool rule_PARAM_LIST() {
    // <param_list> -> EPSILON
    if (t->type == token_PARENTHESES_R) {
        RLOG("<param_list> -> EPSILON\n");
        return true;
    }
    // <param_list> -> <param> <param_next>
    else if (t->type == token_ID) {
        RLOG("<param_list> -> <param> <param_next>\n");
        return rule_PARAM() && rule_PARAM_NEXT();
    }
    return false;
}

bool rule_PARAM_NEXT() {
    // <param_next> -> , <param> <param_next>
    if (t->type == token_COMMA) {
        RLOG("<param_next> -> , <param> <param_next>\n");
        t = mock_recursive_nextToken();
        return rule_PARAM() && rule_PARAM_NEXT();
    }
    // <param_next> -> EPSILON
    else if (t->type == token_PARENTHESES_R) {
        RLOG("<param_next> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_PARAM() {
    // <param> -> id id : <type>
    RLOG("<param> -> id id : <type>\n");
    if (t->type != token_ID) {
        return false;
    }
    t = mock_recursive_nextToken();
    if (t->type != token_ID) {
        return false;
    }
    t = mock_recursive_nextToken();
    if (t->type != token_COLON) {
        return false;
    }
    t = mock_recursive_nextToken();
    return t->type == token_TYPE;
}

bool rule_RETURN_TYPE() {
    // <return_type> -> "-> type"
    if (t->type == token_ARROW) {
        RLOG("<return_type> -> -> type\n");
        t = mock_recursive_nextToken();
        if (t->type == token_TYPE) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    // <return_type> -> EPSILON
    else if (t->type == token_BRACKET_L) {
        RLOG("<return_type> -> EPSILON");
        return true;
    }
    return false;
}

bool rule_FUNC_STAT_LIST() {
    // <func_stat_list> -> <func_stat> EOL <func_stat_list>
    if (t->type == token_EOL ||
        t->type == token_LET ||
        t->type == token_VAR ||
        t->type == token_IF ||
        t->type == token_WHILE ||
        t->type == token_ID ||
        t->type == token_RETURN) {
        RLOG("<func_stat_list> -> <func_stat> EOL <func_stat_list>\n");
        if (rule_FUNC_STAT()) {
            t = mock_recursive_nextToken();
            if (t->type == token_EOL) {
                RLOG("EOL\n");
                t = mock_recursive_nextToken();
                return rule_FUNC_STAT_LIST();
            }
        }
    }
    // <func_stat_list> -> EPSILON    
    if (t->type == token_BRACKET_R) {
        RLOG("<func_stat_list> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_FUNC_STAT() {
    switch (t->type) {
    case token_LET:
    case token_VAR:
        RLOG("<func_stat> -> <let_or_var> <var_assignment>\n");
        return rule_LET_OR_VAR() && rule_VAR_ASSIGNMENT();
    case token_ID:
        RLOG("<func_stat> -> id <after_id>\n");
        t = mock_recursive_nextToken();
        return rule_AFTER_ID();
    case token_RETURN:
        // <func_stat> -> <return_stat>
        RLOG("<func_stat> -> <return_stat>\n");
        return rule_RETURN_STAT();
    case token_IF:
        RLOG("<func_stat> -> if <condition> { <stat_list> } else { <stat_list> }\n");
        t = mock_recursive_nextToken();
        if (!rule_CONDITION()) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_BRACKET_L) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_BRACKET_R) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_ELSE) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_BRACKET_L) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_BRACKET_R) {
            return false;
        }
        return true;
    case token_WHILE:
        RLOG("<func_stat> -> while <expression> { <stat_list> }\n");
        t = mock_recursive_nextToken();
        stash = t;
        if (!rule_EXPRESSION()) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_BRACKET_L) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        t = mock_recursive_nextToken();
        if (t->type != token_BRACKET_R) {
            return false;
        }
        return true;
    case token_EOL:
        RLOG("<func_stat> -> EPSILON\n");
        return true;
    default:
        RLOG("ERROR: <func_stat>\n");
        return false;
    }
}

bool rule_RETURN_STAT() {

    // <return_stat> -> return <ret_val> EOL <func_stat_list>
    if (t->type == token_RETURN) {
        RLOG("<return_stat> -> return <ret_val> EOL <func_stat_list>\n");
        t = mock_recursive_nextToken();
        if (rule_RET_VAL()) {
            t = mock_recursive_nextToken();
            if (t->type == token_EOL) {
                return rule_FUNC_STAT_LIST();
            }
        }
    }
    // <return_stat> -> EPSILON
    else if (t->type == token_EOL) {
        RLOG("<return_stat> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_RET_VAL() {
    // <ret_val> -> <expression>
    if (t->type == token_ID || t->type == token_PARENTHESES_L || t->type == token_CONST ) {
        RLOG("<ret_val> -> <expression>\n");
        return rule_EXPRESSION();
    }
    // <ret_value> -> EPSILON
    else if (t->type == token_EOL) {
        RLOG("<ret_val> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_CONDITION() {
    //<condition> -> <expression>
    if (t->type == token_ID || t->type == token_PARENTHESES_L || t->type == token_CONST ) {
        RLOG("<condition> -> <expression>\n");
        return rule_EXPRESSION();
    }
    // <condtion> -> let id
    if (t->type == token_LET) {
        RLOG("<condition> -> let id\n");
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    return false;
}














