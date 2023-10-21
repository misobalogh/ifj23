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


// #define token_is t->type == 

tokenStruct* t;


bool rule_PROGRAM() {
    // <program> -> <stat_list> EOF
    t = mock_recursive_nextToken();
    RLOG("rule_PROGRAM\n");
    if (t->type == token_EOL ||
        t->type == token_LET ||
        t->type == token_VAR ||
        t->type == token_IF ||
        t->type == token_WHILE ||
        t->type == token_ID ||
        t->type == token_FUNC ||
        t->type == token_EXPRESSION) {
        if (rule_STAT_LIST()) {
            RLOG("EOF\n");
            return t->type == token_EOF;
        }
    }
    return false;
}

bool rule_FUNCTION() {
    RLOG("rule_FUNCTION\n");
    // <function> -> func id ( <param_list> ) <return_type> { <func_stat_list> }
    if (t->type == token_FUNC) {
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            if (t->type == token_PARENTHESES_L) {
                t = mock_recursive_nextToken();
                if (rule_PARAM_LIST()) {
                    t = mock_recursive_nextToken();
                    if (t->type == token_PARENTHESES_R) {
                        t = mock_recursive_nextToken();
                        if (rule_RETURN_TYPE()) {
                            if (t->type == token_BRACKET_L) {
                                t = mock_recursive_nextToken();
                                if (rule_FUNC_STAT_LIST()) {
                                    t = mock_recursive_nextToken();
                                    return t->type == token_BRACKET_R;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}


bool rule_RETURN_TYPE() {
    RLOG("rule_RETURN_TYPE\n");
    // <return_type> -> "-> type"
    if (t->type == token_ARROW) {
        t = mock_recursive_nextToken();
        if (t->type == token_TYPE) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    // <return_type> -> EPSILON
    else if (t->type == token_BRACKET_L) {
        return true;
    }
    return false;
}


bool rule_FUNC_STAT_LIST() {
    RLOG("rule_FUNC_STAT_LIST\n");
    // <func_stat_list> -> <func_stat> EOL <func_stat_list>
    if (t->type == token_EOL ||
        t->type == token_LET ||
        t->type == token_VAR ||
        t->type == token_IF ||
        t->type == token_WHILE ||
        t->type == token_ID ||
        t->type == token_RETURN ||
        t->type == token_EXPRESSION) {
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
    else if (t->type == token_BRACKET_R) {
        return true;
    }
    return false;
}


bool rule_FUNC_STAT() {
    switch (t->type) {
    case token_RETURN:
        return rule_RETURN_STAT();
    case token_LET:
    case token_VAR:
        return rule_VAR_DEFINITION() && rule_VAR_ASSIGNMENT();
    case token_ID:
        t = mock_recursive_nextToken();
        if (t->type == token_ASSIGN) {
            return rule_EXPRESSION();
        }
        return false;
    case token_EXPRESSION:
        t = mock_recursive_nextToken();
        return true; // replace with switching to precedence parser
    case token_IF:
        t = mock_recursive_nextToken();
        if (rule_CONDITION()) {
            t = mock_recursive_nextToken();
            if (t->type == token_PARENTHESES_L) {
                t = mock_recursive_nextToken();
                if (rule_FUNC_STAT_LIST()) {
                    t = mock_recursive_nextToken();
                    if (t->type == token_PARENTHESES_R) {
                        t = mock_recursive_nextToken();
                        if (t->type == token_ELSE) {
                            t = mock_recursive_nextToken();
                            if (t->type == token_PARENTHESES_L) {
                                t = mock_recursive_nextToken();
                                if (rule_FUNC_STAT_LIST()) {
                                    t = mock_recursive_nextToken();
                                    return t->type == token_PARENTHESES_R;

                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
    case token_WHILE:
        if (rule_EXPRESSION()) {
            t = mock_recursive_nextToken();
            if (t->type == token_PARENTHESES_L) {
                t = mock_recursive_nextToken();
                if (rule_FUNC_STAT_LIST()) {
                    t = mock_recursive_nextToken();
                    return t->type == token_PARENTHESES_R;
                }
            }
        }
        return false;
    case token_EOL:
        return true;
    default:
        return false;
    }
    return false;
}


bool rule_CONDITION() {
    if (t->type == token_EXPRESSION) {
        t = mock_recursive_nextToken();
        return true;
    }
    if (t->type == token_LET) {
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    return false;
}

bool rule_RETURN_STAT() {
    // <return_stat> -> return <ret_val> EOL <func_stat_list>
    if (t->type == token_RETURN) {
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
        return true;
    }
    return false;
}


bool rule_RET_VAL() {
    // <ret_val> -> <expression>
    if (t->type == token_EXPRESSION) {
        t = mock_recursive_nextToken();
        return true;
    }
    // <ret_value> -> EPSILON
    else if (t->type == token_EOL) {
        return true;
    }
    return false;
}

bool rule_PARAM_LIST() {
    // <param_list> -> EPSILON
    if (t->type == token_PARENTHESES_R) {
        return true;
    }
    // <param_list> -> <param> <param_next>
    else if (t->type == token_ID) {
        return rule_PARAM() && rule_PARAM_NEXT();
    }
    return false;
}

bool rule_PARAM_NEXT() {
    // <param_next> -> , <param> <param_next>
    if (t->type == token_COMMA) {
        t = mock_recursive_nextToken();
        return rule_PARAM() && rule_PARAM_NEXT();
    }
    // <param_next> -> EPSILON
    else if (t->type == token_PARENTHESES_R) {
        return true;
    }
    return false;
}


bool rule_PARAM() {
    // <param> -> id id : <type>
    if (t->type == token_ID) {
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            if (t->type == token_COLON) {
                t = mock_recursive_nextToken();
                return t->type == token_TYPE;
            }
        }
    }
    return false;
}


bool rule_STAT_LIST() {
    //  <stat_list> -> <statement> EOL <stat_list>
    if (t->type == token_EOL ||
        t->type == token_LET ||
        t->type == token_VAR ||
        t->type == token_IF ||
        t->type == token_WHILE ||
        t->type == token_ID ||
        t->type == token_FUNC ||
        t->type == token_EXPRESSION) {
        if (rule_STATEMENT()) {
            t = mock_recursive_nextToken();
            if (t->type == token_EOL) {
                t = mock_recursive_nextToken();
                return rule_STAT_LIST();
            }
        }
    }
    // <stat_list> -> EPSILON
    else if (t->type == token_EOF /*|| t->type == token_EOL*/) {
        return true;
    }
    return false;
}


bool rule_STATEMENT() {
    switch (t->type) {
        // <statement> -> <function> EOL <stat_list>    
    case token_FUNC:
        if (rule_FUNCTION()) {
            t = mock_recursive_nextToken();
            if (t->type == token_EOL) {
                t = mock_recursive_nextToken();
                return rule_STAT_LIST();
            }
        }
        return false;
        // <statement> -> <var_definition> <var_assigment>
    case token_LET:
    case token_VAR:
        return rule_VAR_DEFINITION() && rule_VAR_ASSIGNMENT();
        //  <statement> -> id = <expression>
    case token_ID:
        t = mock_recursive_nextToken();
        if (t->type == token_ASSIGN) {
            return rule_EXPRESSION();
        }
        return false;
        //  <statement> -> <expression> 
    case token_EXPRESSION:
        t = mock_recursive_nextToken();
        return true; // replace with switching to precedence parser
        // <statement> -> if <condition> { <stat_list> } else { <stat_list> }
    case token_IF:
        t = mock_recursive_nextToken();
        if (rule_CONDITION()) {
            t = mock_recursive_nextToken();
            if (t->type == token_PARENTHESES_L) {
                t = mock_recursive_nextToken();
                if (rule_STAT_LIST()) {
                    t = mock_recursive_nextToken();
                    if (t->type == token_PARENTHESES_R) {
                        t = mock_recursive_nextToken();
                        if (t->type == token_ELSE) {
                            t = mock_recursive_nextToken();
                            if (t->type == token_PARENTHESES_L) {
                                t = mock_recursive_nextToken();
                                if (rule_STAT_LIST()) {
                                    t = mock_recursive_nextToken();
                                    return t->type == token_PARENTHESES_R;

                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
        // <statement> -> while <expression> { <stat_list> }
    case token_WHILE:
        if (rule_EXPRESSION()) {
            t = mock_recursive_nextToken();
            if (t->type == token_PARENTHESES_L) {
                t = mock_recursive_nextToken();
                if (rule_STAT_LIST()) {
                    t = mock_recursive_nextToken();
                    return t->type == token_PARENTHESES_R;
                }
            }
        }
        return false;
        // <statement> -> EOL
    case token_EOL:
        return true;
    default:
        return false;
    }
    return false;
}


bool rule_VAR_DEFINITION() {
    // <var_definition> -> let id
    if (t->type == token_LET) {
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    // <var_definition> -> var id
    else if (t->type == token_VAR) {
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    return false;
}

bool rule_VAR_ASSIGNMENT() {
    // <var_assigment> -> : <type> <val_assigment>
    if (t->type == token_COLON) {
        t = mock_recursive_nextToken();
        if (t->type == token_TYPE) {
            return rule_VAL_ASSIGNMENT();
        }
    }
    // <var_assigment> -> = <expression>
    else if (t->type == token_ASSIGN) {
        t = mock_recursive_nextToken();
        if (t->type == token_EXPRESSION) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    return false;
}


bool rule_VAL_ASSIGNMENT() {
    // <val_assigment> -> = <expression>
    if (t->type == token_ASSIGN) {
        t = mock_recursive_nextToken();
        if (t->type == token_EXPRESSION) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    // <val_assigment> -> EPSILON
    else if (t->type == token_EOL) {
        return true;
    }
    return false;
}


bool rule_INPUT_PARAM_LIST() {
    // <input_param_list> -> EPSILON
    if (t->type == token_PARENTHESES_R) {
        return true;
    }
    // <input_param_list> -> <input_param> <input_param_next>
    else if (t->type == token_ID || t->type == token_CONST) {
        return rule_INPUT_PARAM && rule_INPUT_PARAM_NEXT();
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
        t = mock_recursive_nextToken();
        return true;
    }
    // <input_param> -> id <with_name>
    else if (t->type == token_ID) {
        t = mock_recursive_nextToken();
        return rule_WITH_NAME();
    }
    return false;
}


bool rule_WITH_NAME() {
    // <with_name> -> EPSILON 
    if (t->type == token_COMMA ||
        t->type == token_ID ||
        t->type == token_CONST) {
        return true;
    }
    // <with_name> -> : <id_or_const>
    else if (t->type == token_COLON) {
        t = mock_recursive_nextToken();
        return rule_ID_OR_CONST();
    }
    return false;
}

bool rule_ID_OR_CONST() {
    // <id_or_const> -> id
    if(t->type == token_ID) {
        t = mock_recursive_nextToken();
        return true;
    }
    // <id_or_const> -> const
    else if(t->type == token_CONST) {
        t = mock_recursive_nextToken();
        return true;
    }
}



















