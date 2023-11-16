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
#include "semantic_analysis.h"
#include "generator.h"

#define SEMANTIC_CHECK(code) do { if (code != SUCCESS) return code; } while (0)


/**
 * @brief Most recent token from scanner
 */
lex_token t = { .type = token_EMPTY, .value = {0} };

/**
 * @brief Stash for token for expression parser
 * when switching from recursive parser when deciding,
 * whether it is expression or function call
 */
lex_token stash = { .type = token_EMPTY, .value = {0} };

void consume_optional_EOL() {
    if (t.type == token_EOL) {
        RLOG("consume_optional_EOL()\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
    }
}

// ===================== RULES =====================

bool rule_EXPRESSION() {
    RLOG("<expression> => switching to precedence parser\n");
    error_codes retVal = precedenceParser();
    stash.type = token_EMPTY;
    return retVal;
}

bool rule_PROGRAM() {
    // 1. <program> -> <stat_list> EOF
    t = get_next_token();
    LEX_ERR_CHECK();
    RLOG("\n\n<program> -> <stat_list> EOF\n");
    if (t.type == token_EOL ||
        t.type == token_LET ||
        t.type == token_VAR ||
        t.type == token_IF ||
        t.type == token_WHILE ||
        t.type == token_ID ||
        t.type == token_FUNC
        ) {
        if (rule_STAT_LIST()) {
            RLOG("EOF\n");
            return true;
        }
    }
    else if (t.type == token_EOF) {
        RLOG("EOF\n");
        return true;
    }
    return false;
}

bool rule_STAT_LIST() {
    // 2. <stat_list> -> <statement> EOL <stat_list>
    if (t.type == token_LET ||
        t.type == token_VAR ||
        t.type == token_IF ||
        t.type == token_WHILE ||
        t.type == token_ID ||
        t.type == token_FUNC ||
        t.type == token_EOL) {
        RLOG("<stat_list> -> <statement> EOL <stat_list>\n");
        if (rule_STATEMENT()) {
            if (t.type == token_EOL) {
                t = get_next_token();
                LEX_ERR_CHECK();
                return rule_STAT_LIST();
            }
            else if (t.type == token_EOF) {
                return true;
            }
        }
    }
    // 3. <stat_list> -> EPSILON
    else if (t.type == token_EOF) {
        RLOG("<stat_list> -> EPSILON\n");
        return true;
    }
    return false;
}


bool rule_STATEMENT() {
    switch (t.type) {
    case token_LET:
    case token_VAR:
        RLOG("<statement> -> <let_or_var> <var_assignment>\n");
        return rule_LET_OR_VAR() && rule_VAR_ASSIGNMENT();
    case token_ID:
        RLOG("<statement> -> id <after_id>\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return rule_AFTER_ID();
    case token_FUNC:
        RLOG("<statement> -> func id ( <param_list> ) <return_type> { <func_stat_list> }\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_ID) {
            return false;
        }

        SEMANTIC_CHECK(analyseFunctionId(t.value.STR_VAL));

        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_PARENTHESES_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_PARAM_LIST()) {
            return false;
        }
        if (t.type != token_PARENTHESES_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_RETURN_TYPE()) {
            return false;
        }
        SEMANTIC_CHECK(analyseFunctionEnd());
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_FUNC_STAT_LIST()) {
            return false;
        }
        if (t.type != token_BRACKET_R) {
            return false;
        }

        t = get_next_token();
        LEX_ERR_CHECK();
        return true;
    case token_IF:
        RLOG("<statement> -> if <condition> { <brack_stat_list> } else { <brack_stat_list> }\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_CONDITION()) {
            return false;
        }
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        if (t.type != token_BRACKET_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_ELSE) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        if (t.type != token_BRACKET_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        return true;
    case token_WHILE:
        RLOG("<statement> -> while <expression> { <brack_stat_list> }\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_EXPRESSION()) {
            return false;
        }
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        if (t.type != token_BRACKET_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        return true;
    case token_EOL:
        RLOG("<statement> -> EPSILON\n");
        return true;
    default:
        RLOG("ERROR: <statement>\n");
        return false;
    }
}


bool rule_BRACK_STAT_LIST() {
    // 2. <brack_stat_list> -> <brack_statement> EOL <between_brackets>
    if (t.type == token_LET ||
        t.type == token_VAR ||
        t.type == token_IF ||
        t.type == token_WHILE ||
        t.type == token_ID ||
        t.type == token_EOL
        ) {
        RLOG("<brack_stat_list> -> <brack_statement> EOL <brack_stat_list>\n");
        if (rule_BRACK_STATEMENT()) {
            if (t.type == token_EOL) {
                t = get_next_token();
                LEX_ERR_CHECK();
                return rule_BRACK_STAT_LIST();
            }
        }
    }
    if (t.type == token_BRACKET_R) {
        RLOG("<brack_stat_list> -> EPSILON\n");
        return true;
    }
    return false;
}


bool rule_BRACK_STATEMENT() {
    switch (t.type) {
    case token_LET:
    case token_VAR:
        RLOG("<brack_statement> -> <let_or_var> <var_assignment>\n");
        return rule_LET_OR_VAR() && rule_VAR_ASSIGNMENT();
    case token_ID:
        RLOG("<brack_statement> -> id <after_id>\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return rule_AFTER_ID();
    case token_IF:
        RLOG("<brack_statement> -> if <condition> { <brack_stat_list> } else { <brack_stat_list> }\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_CONDITION()) {
            return false;
        }
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        if (t.type != token_BRACKET_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_ELSE) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        if (t.type != token_BRACKET_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        return true;
    case token_WHILE:
        RLOG("<brack_statement> -> while <expression> { <brack_stat_list> }\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_EXPRESSION()) {
            return false;
        }
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        if (t.type != token_BRACKET_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        return true;
    case token_EOL:
        RLOG("<brack_statement> -> EPSILON\n");
        return true;
    default:
        RLOG("ERROR: <brack_statement>\n");
        return false;
    }
}


bool rule_LET_OR_VAR() {
    if (t.type == token_LET) {
        RLOG("<let_or_var> -> let id\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type == token_ID) {
            t = get_next_token();
            LEX_ERR_CHECK();
            consume_optional_EOL();
            return true;
        }
    }
    else if (t.type == token_VAR) {
        RLOG("<let_or_var> -> var id\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type == token_ID) {
            t = get_next_token();
            LEX_ERR_CHECK();
            consume_optional_EOL();
            return true;
        }
    }
    return false;
}

bool rule_VAR_ASSIGNMENT() {
    // <var_assigment> -> : type <val_assigment>
    if (t.type == token_COLON) {
        RLOG("<var_assignment> -> : type <val_assignment>\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (rule_TYPE()) {
            return rule_VAL_ASSIGNMENT();
        }
    }
    // <var_assigment> -> = id <fn_or_exp>
    else if (t.type == token_ASSIGN) {
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type == token_ID) {
            RLOG("<var_assignment> -> = id <fn_or_exp>\n");
            stash = t;
            t = get_next_token();
            LEX_ERR_CHECK();
            return rule_FN_OR_EXP();
        }
        else if (t.type == token_CONST
            || t.type == token_CONST_WHOLE_NUMBER
            || t.type == token_CONST_DEC_NUMBER
            || t.type == token_CONST_SCIENTIFIC_NOTATION
            || t.type == token_PARENTHESES_L
            || t.type == token_TYPE_STRING_LINE) {
            RLOG("<var_assignment> -> = const <expression>\n");
            return rule_EXPRESSION();
        }
    }
    return false;
}

bool rule_VAL_ASSIGNMENT() {
    // <val_assigment> -> = id <fn_or_exp>
    if (t.type == token_ASSIGN) {
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type == token_ID) {
            RLOG("<val_assignment> -> = id <fn_or_exp>\n");
            stash = t;
            t = get_next_token();
            LEX_ERR_CHECK();

            return rule_FN_OR_EXP();
        }
        else if (t.type == token_CONST
            || t.type == token_CONST_WHOLE_NUMBER
            || t.type == token_CONST_DEC_NUMBER
            || t.type == token_CONST_SCIENTIFIC_NOTATION
            || t.type == token_PARENTHESES_L
            || t.type == token_TYPE_STRING_LINE) {
            RLOG("<val_assignment> -> = const <expression>\n");
            return rule_EXPRESSION();
        }
    }
    // <val_assigment> -> EPSILON
    else if (t.type == token_EOL) {
        RLOG("<val_assignment> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_FN_OR_EXP() {
    // <fn_or_exp> -> id/const
    if (t.type == token_EOL || t.type == token_EOF) {
        RLOG("<fn_or_exp> -> id\n");
        stash.type = token_EMPTY;
        return true;
    }
    // <fn_or_exp> -> <expression> 
    else if ((t.type >= token_OP_START && t.type <= token_OP_END)) {
        RLOG("<fn_or_exp> -> <expression>\n");
        return rule_EXPRESSION();
    }
    // <fn_or_exp> -> ( <input_param_list> )
    else if (t.type == token_PARENTHESES_L) {
        RLOG("<fn_or_exp> -> ( <input_param_list> )\n");
        stash.type = token_EMPTY;
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_INPUT_PARAM_LIST()) {
            return false;
        }
        if (t.type != token_PARENTHESES_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        return true;
    }
    return false;
}

bool rule_AFTER_ID() {
    char* left = t.value.STR_VAL;

    // <after_id> -> = id <fn_or_exp> 
    if (t.type == token_ASSIGN) {
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();

        if (t.type == token_ID) {
            RLOG("<after_id> -> = id <fn_or_exp>\n");
            stash = t;
            t = get_next_token();
            LEX_ERR_CHECK();
            // TODO: analyse assignment
            return rule_FN_OR_EXP();
        }
        else if (t.type == token_CONST
            || t.type == token_CONST_WHOLE_NUMBER
            || t.type == token_CONST_DEC_NUMBER
            || t.type == token_CONST_SCIENTIFIC_NOTATION
            || t.type == token_PARENTHESES_L
            || t.type == token_TYPE_STRING_LINE) {
            RLOG("<after_id> -> = const <expression>\n");
            bool retval = rule_EXPRESSION();
            return retval;
        }
    }
    // <after_id> -> ( <input_param_list> ) 
    else if (t.type == token_PARENTHESES_L) {
        RLOG("<after_id> -> ( <input_param_list> )\n");
        t = get_next_token();
        SEMANTIC_CHECK(analyseCallId(t.value.STR_VAL));
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_INPUT_PARAM_LIST()) {
            return false;
        }
        if (t.type != token_PARENTHESES_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        return true;
    }
    return false;
}

bool rule_INPUT_PARAM_LIST() {
    // <input_param_list> -> EPSILON
    if (t.type == token_PARENTHESES_R) {
        RLOG("<input_param_list> -> EPSILON\n");
        return true;
    }
    // <input_param_list> -> <input_param> <input_param_next>
    else if (t.type == token_ID
        || t.type == token_CONST
        || t.type == token_CONST_WHOLE_NUMBER
        || t.type == token_CONST_DEC_NUMBER
        || t.type == token_CONST_SCIENTIFIC_NOTATION
        || t.type == token_TYPE_STRING_LINE) {
        RLOG("<input_param_list> -> <input_param> <input_param_next>\n");
        return rule_INPUT_PARAM() && rule_INPUT_PARAM_NEXT();
    }
    return false;
}

bool rule_INPUT_PARAM_NEXT() {
    // <input_param_next> -> , <input_param> <input_param_next>
    if (t.type == token_COMMA) {
        RLOG("<input_param_next> -> , <input_param> <input_param_next>\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return rule_INPUT_PARAM() && rule_INPUT_PARAM_NEXT();
    }
    // <input_param_next> -> EPSILON
    else if (t.type == token_PARENTHESES_R) {
        RLOG("<input_param_next> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_INPUT_PARAM() {
    // <input_param> -> const        
    if (t.type == token_CONST
        || t.type == token_CONST_WHOLE_NUMBER
        || t.type == token_CONST_DEC_NUMBER
        || t.type == token_CONST_SCIENTIFIC_NOTATION
        || t.type == token_TYPE_STRING_LINE) {
        RLOG("<input_param> -> const\n");
        SEMANTIC_CHECK(analyseCallLabel(NULL));
        /* analyseCallParamConst(t.value); */
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return true;
    }
    // <input_param> -> id <with_name>
    else if (t.type == token_ID) {
        RLOG("<input_param> -> id <with_name>\n");
        SEMANTIC_CHECK(analyseCallLabel(t.value.STR_VAL));
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return rule_WITH_NAME();
    }
    return false;
}

bool rule_WITH_NAME() {
    // <with_name> -> EPSILON 
    if (t.type == token_COMMA ||
        t.type == token_PARENTHESES_R) {
        RLOG("<with_name> -> EPSILON\n");

        return true;
    }
    // <with_name> -> : <id_or_const>
    else if (t.type == token_COLON) {
        RLOG("<with_name> -> : <id_or_const>\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return rule_ID_OR_CONST();
    }
    return false;
}

bool rule_ID_OR_CONST() {
    // <id_or_const> -> id
    if (t.type == token_ID) {
        RLOG("<id_or_const> -> id\n");
        SEMANTIC_CHECK(analyseCallParam(t.value.STR_VAL));
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return true;
    }
    // <id_or_const> -> const
    else if (t.type == token_CONST
        || t.type == token_CONST_WHOLE_NUMBER
        || t.type == token_CONST_DEC_NUMBER
        || t.type == token_CONST_SCIENTIFIC_NOTATION
        || t.type == token_TYPE_STRING_LINE) {
        RLOG("<id_or_const> -> const\n");
        /* analyseCallParamConst(t->value); */
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return true;
    }
    return false;
}

bool rule_PARAM_LIST() {
    // <param_list> -> EPSILON
    if (t.type == token_PARENTHESES_R) {
        RLOG("<param_list> -> EPSILON\n");
        return true;
    }
    // <param_list> -> <param> <param_next>
    else if (t.type == token_ID || t.type == token_UNDERSCORE) {
        RLOG("<param_list> -> <param> <param_next>\n");
        return rule_PARAM() && rule_PARAM_NEXT();
    }
    return false;
}

bool rule_PARAM_NEXT() {
    // <param_next> -> , <param> <param_next>
    if (t.type == token_COMMA) {
        RLOG("<param_next> -> , <param> <param_next>\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return rule_PARAM() && rule_PARAM_NEXT();
    }
    // <param_next> -> EPSILON
    else if (t.type == token_PARENTHESES_R) {
        RLOG("<param_next> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_PARAM() {
    // <param> -> <id_or_underscore> id : <type>
    RLOG("<param> -> <id_or_underscore> id : <type>\n");
    if (!rule_ID_OR_UNDERSCORE()) {
        return false;
    }
    if (t.type != token_ID) {
        return false;
    }
    SEMANTIC_CHECK(analyseFunctionParamName(t.value.STR_VAL));
    t = get_next_token();
    LEX_ERR_CHECK();
    consume_optional_EOL();
    if (t.type != token_COLON) {
        return false;
    }
    t = get_next_token();
    LEX_ERR_CHECK();
    consume_optional_EOL();
    SEMANTIC_CHECK(analyseFunctionParamType(t.type));
    return rule_TYPE();
}

bool rule_ID_OR_UNDERSCORE() {
    // <id_or_underscore> -> id
    if (t.type == token_ID) {
        RLOG("<id_or_underscore> -> id\n");
        SEMANTIC_CHECK(analyseFunctionParamLabel(t.value.STR_VAL));
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return true;
    }
    // <id_or_underscore> -> _
    else if (t.type == token_UNDERSCORE) {
        RLOG("<id_or_underscore> -> _\n");
        SEMANTIC_CHECK(analyseFunctionParamLabel(NULL));
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return true;
    }
    return false;
}

bool rule_RETURN_TYPE() {
    // <return_type> -> ". type"
    if (t.type == token_ARROW) {
        RLOG("<return_type> -> -> <type>\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        SEMANTIC_CHECK(analyseFunctionType(t.type));
        if (rule_TYPE()) {
            return true;
        }
    }
    // <return_type> -> EPSILON
    else if (t.type == token_BRACKET_L) {
        SEMANTIC_CHECK(analyseFunctionType(token_EMPTY));
        RLOG("<return_type> -> EPSILON");
        return true;
    }
    return false;
}

bool rule_FUNC_STAT_LIST() {
    // <func_stat_list> -> <func_stat> EOL <func_stat_list>
    if (t.type == token_EOL ||
        t.type == token_LET ||
        t.type == token_VAR ||
        t.type == token_IF ||
        t.type == token_WHILE ||
        t.type == token_ID ||
        t.type == token_RETURN) {
        RLOG("<func_stat_list> -> <func_stat> EOL <func_stat_list>\n");
        if (rule_FUNC_STAT()) {
            if (t.type == token_EOL) {
                t = get_next_token();
                LEX_ERR_CHECK();
                return rule_FUNC_STAT_LIST();
            }
        }
    }
    // <func_stat_list> -> EPSILON    
    if (t.type == token_BRACKET_R) {
        RLOG("<func_stat_list> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_FUNC_STAT() {
    switch (t.type) {
    case token_LET:
    case token_VAR:
        RLOG("<func_stat> -> <let_or_var> <var_assignment>\n");
        return rule_LET_OR_VAR() && rule_VAR_ASSIGNMENT();
    case token_ID:
        RLOG("<func_stat> -> id <after_id>\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        return rule_AFTER_ID();
    case token_RETURN:
        // <func_stat> -> <return_stat>
        RLOG("<func_stat> -> <return_stat>\n");
        return rule_RETURN_STAT();
    case token_IF:
        RLOG("<func_stat> -> if <condition> { <func_stat_list> } else { <func_stat_list> }\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_CONDITION()) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_BRACKET_R) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_ELSE) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_BRACKET_R) {
            return false;
        }
        return true;
    case token_WHILE:
        RLOG("<func_stat> -> while <expression> { <func_stat_list> }\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        stash = t;
        if (!rule_EXPRESSION()) {
            return false;
        }
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (!rule_BRACK_STAT_LIST()) {
            return false;
        }
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type != token_BRACKET_R) {
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
    if (t.type == token_RETURN) {
        RLOG("<return_stat> -> return <ret_val> EOL <func_stat_list>\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (rule_RET_VAL()) {
            if (t.type == token_EOL) {
                return rule_FUNC_STAT_LIST();
            }
        }
    }
    // <return_stat> -> EPSILON
    else if (t.type == token_EOL) {
        RLOG("<return_stat> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_RET_VAL() {
    // <ret_val> -> <expression>
    if (t.type == token_ID
        || t.type == token_PARENTHESES_L
        || t.type == token_CONST
        || t.type == token_CONST_WHOLE_NUMBER
        || t.type == token_CONST_DEC_NUMBER
        || t.type == token_CONST_SCIENTIFIC_NOTATION
        || t.type == token_TYPE_STRING_LINE) {
        RLOG("<ret_val> -> <expression>\n");
        return rule_EXPRESSION();
    }
    // <ret_value> -> EPSILON
    else if (t.type == token_EOL) {
        RLOG("<ret_val> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_CONDITION() {
    //<condition> -> <expression>
    if (t.type == token_ID
        || t.type == token_CONST
        || t.type == token_CONST_WHOLE_NUMBER
        || t.type == token_CONST_DEC_NUMBER
        || t.type == token_CONST_SCIENTIFIC_NOTATION
        || t.type == token_PARENTHESES_L
        || t.type == token_TYPE_STRING_LINE) {
        RLOG("<condition> -> <expression>\n");
        return rule_EXPRESSION();
    }
    // <condtion> -> let id
    if (t.type == token_LET) {
        RLOG("<condition> -> let id\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        consume_optional_EOL();
        if (t.type == token_ID) {
            t = get_next_token();
            LEX_ERR_CHECK();
            consume_optional_EOL();
            return true;
        }
    }
    return false;
}

bool rule_TYPE() {
    if (t.type == token_TYPE_STRING ||
        t.type == token_TYPE_STRING_Q ||
        t.type == token_TYPE_INT ||
        t.type == token_TYPE_INT_Q ||
        t.type == token_TYPE_DOUBLE ||
        t.type == token_TYPE_DOUBLE_Q) {
        RLOG("<type> -> type\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        return true;
    }
    return false;
}












