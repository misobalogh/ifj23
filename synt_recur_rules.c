/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_recur_rules.c
* Description: rules for top-down syntactic analysis
// * Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 21.10.2023

* Comments:

***************************************************************/

#include "global_variables.h"
#include "symtablestack.h"
#include "synt_analysis.h"
#include "macros.h"
#include "synt_recur_rules.h"
#include "semantic_analysis.h"
#include "generator.h"



void getToken() {
    if (stash.type != token_EMPTY) {
        t = stash;
        stash.type = token_EMPTY;
    }
    else {
        t = get_next_token();
        consume_optional_EOL();
        LEX_ERR_CHECK();
    }
}


bool EOL_flag = false;

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
    EOL_flag = false;
    while (t.type == token_EOL) {
        RLOG("EOL consumed\n");
        t = get_next_token();
        LEX_ERR_CHECK();
        EOL_flag = true;
    }
}

// ===================== RULES =====================

bool rule_EXPRESSION() {
    RLOG("<expression> => switching to precedence parser\n");
    analyseExprBegin();
    if (!precedenceParser()) {
        return false;
    }
    Type exprType = analyseExprEnd();

    analyseReturn(exprType);

    analyseAssignType(exprType);
    analyseAssignEnd();

    analyseReassignType(exprType);
    analyseReassignEnd();

    return true;
}

bool rule_PROGRAM() {
    // 1. <program> -> <stat_list> EOF

    genSubstring();

    getToken();
    RLOG("\n\n<program> -> <stat_list> EOF\n");
    if (t.type == token_LET ||
        t.type == token_VAR ||
        t.type == token_IF ||
        t.type == token_WHILE ||
        t.type == token_ID ||
        t.type == token_FUNC
        ) {
        if (rule_STAT_LIST()) {
            RLOG("EOF\n");
            RLOG("Returned TRUE");
            return true;
        }
    }
    else if (t.type == token_EOF) {
        RLOG("Returned TRUE");
        RLOG("DONE");
        return true;
    }
    RLOG("Returned FALSE");
    return false;
}

bool rule_STAT_LIST() {
    // 2. <stat_list> -> <statement> EOL <stat_list>
    if (t.type == token_LET ||
        t.type == token_VAR ||
        t.type == token_IF ||
        t.type == token_WHILE ||
        t.type == token_ID ||
        t.type == token_FUNC) {
        RLOG("<stat_list> -> <statement> EOL <stat_list>\n");
        if (rule_STATEMENT()) {
            if (EOL_flag == true) {
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
    prepareStatement();

    switch (t.type) {
    case token_LET:
    case token_VAR:
        RLOG("<statement> -> <let_or_var> <var_assignment>\n");
        analyseAssignBegin();
        return rule_LET_OR_VAR() && rule_VAR_ASSIGNMENT();
    case token_ID:
        RLOG("<statement> -> id <after_id>\n");
        analyseReassignStart();
        analyseReassignId(t.value.STR_VAL);
        analyseCallFnId(t.value.STR_VAL);
        getToken();
        return rule_AFTER_ID();
    case token_FUNC:
        RLOG("<statement> -> func id ( <param_list> ) <return_type> { <func_stat_list> }\n");

        genMainJump();

        getToken();
        if (t.type != token_ID) {
            return false;
        }

        String idname;
        stringInit(&idname, t.value.STR_VAL);

        analyseFunctionId(t.value.STR_VAL);
        getToken();
        if (t.type != token_PARENTHESES_L) {
            return false;
        }
        getToken();
        if (rule_PARAM_LIST() == false) {
            return false;
        }
        if (t.type != token_PARENTHESES_R) {
            return false;
        }
        getToken();

        if (rule_RETURN_TYPE() == false) {
            return false;
        }
        analyseFunctionEnd();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();

        symtableStackPush(global_symtableStack, true);
        pushFnParams(stringCStr(&idname));

        setCurrentFunction(&idname);
        if (rule_FUNC_STAT_LIST() == false) {
            return false;
        }
        if (t.type != token_BRACKET_R) {
            return false;
        }
        stringClear(&idname);
        setCurrentFunction(&idname);
        stringFree(&idname);

        genReturn();
        symtableStackPop(global_symtableStack);

        genMainLabel();

        getToken();
        LEX_ERR_CHECK();
        return true;
    case token_IF:
        RLOG("<statement> -> if <condition> { <brack_stat_list> } else { <brack_stat_list> }\n");
        getToken();

        analyseIfLetBegin();

        if (rule_CONDITION() == false) {
            return false;
        }
        genIfBegin();


        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();


        symtableStackPush(global_symtableStack, false);
        pushIfLet();
        genIfBlock();
        if (rule_BRACK_STAT_LIST() == false) {
            return false;
        }
        symtableStackPop(global_symtableStack);

        if (t.type != token_BRACKET_R) {
            return false;
        }
        getToken();

        if (t.type != token_ELSE) {
            return false;
        }
        getToken();

        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();

        symtableStackPush(global_symtableStack, false);
        genIfElse();
        if (rule_FUNC_STAT_LIST() == false) {
            return false;
        }
        symtableStackPop(global_symtableStack);
        genIfEnd();

        if (t.type != token_BRACKET_R) {
            return false;
        }
        getToken();

        return true;

    case token_WHILE:
        RLOG("<statement> -> while <expression> { <brack_stat_list> }\n");
        getToken();

        genWhileBegin();

        if (rule_EXPRESSION() == false) {
            return false;
        }
        analyseCondition();

        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();

        symtableStackPush(global_symtableStack, false);
        genWhileStats();
        if (rule_BRACK_STAT_LIST() == false) {
            return false;
        }

        genWhileEnd();

        symtableStackPop(global_symtableStack);

        if (t.type != token_BRACKET_R) {
            return false;
        }
        getToken();
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
        t.type == token_ID) {
        RLOG("<brack_stat_list> -> <brack_statement> EOL <brack_stat_list>\n");
        if (rule_BRACK_STATEMENT()) {
            if (EOL_flag == true) {
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
    prepareStatement();

    switch (t.type) {
    case token_LET:
    case token_VAR:
        RLOG("<brack_statement> -> <let_or_var> <var_assignment>\n");
        analyseAssignBegin();
        return rule_LET_OR_VAR() && rule_VAR_ASSIGNMENT();
    case token_ID:
        RLOG("<brack_statement> -> id <after_id>\n");
        analyseReassignStart();
        analyseReassignId(t.value.STR_VAL);
        analyseCallFnId(t.value.STR_VAL);
        getToken();

        return rule_AFTER_ID();
    case token_IF:
        RLOG("<brack_statement> -> if <condition> { <brack_stat_list> } else { <brack_stat_list> }\n");
        getToken();

        analyseIfLetBegin();
        if (rule_CONDITION() == false) {
            return false;
        }
        genIfBegin();
        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();

        symtableStackPush(global_symtableStack, false);
        pushIfLet();
        genIfBlock();
        if (rule_BRACK_STAT_LIST() == false) {
            return false;
        }
        symtableStackPop(global_symtableStack);

        if (t.type != token_BRACKET_R) {
            return false;
        }
        getToken();

        if (t.type != token_ELSE) {
            return false;
        }
        getToken();

        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();

        symtableStackPush(global_symtableStack, false);
        genIfElse();
        if (rule_BRACK_STAT_LIST() == false) {
            return false;
        }
        symtableStackPop(global_symtableStack);
        genIfEnd();
        if (t.type != token_BRACKET_R) {
            return false;
        }
        getToken();

        return true;

    case token_WHILE:
        RLOG("<brack_statement> -> while <expression> { <brack_stat_list> }\n");
        getToken();

        genWhileBegin();
        if (rule_EXPRESSION() == false) {
            return false;
        }
        analyseCondition();

        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();

        symtableStackPush(global_symtableStack, false);
        genWhileStats();
        if (rule_BRACK_STAT_LIST() == false) {
            return false;
        }
        genWhileEnd();
        symtableStackPop(global_symtableStack);
        if (t.type != token_BRACKET_R) {
            return false;
        }
        getToken();
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
        analyseAssignLet(true);
        getToken();

        if (t.type == token_ID) {
            analyseAssignId(t.value.STR_VAL);
            getToken();
            LEX_ERR_CHECK();
            consume_optional_EOL();
            return true;
        }
    }
    else if (t.type == token_VAR) {
        RLOG("<let_or_var> -> var id\n");
        analyseAssignLet(false);
        getToken();
        if (t.type == token_ID) {
            analyseAssignId(t.value.STR_VAL);
            getToken();
            return true;
        }
    }
    return false;
}

bool rule_VAR_ASSIGNMENT() {
    // <var_assigment> -> : type <val_assigment>
    if (t.type == token_COLON) {
        RLOG("<var_assignment> -> : type <val_assignment>\n");
        getToken();
        if (rule_TYPE()) {
            return rule_VAL_ASSIGNMENT();
        }
    }
    // <var_assigment> -> = id <fn_or_exp>
    else if (t.type == token_ASSIGN) {
        getToken();

        if (t.type == token_ID) {
            RLOG("<var_assignment> -> = id <fn_or_exp>\n");

            analyseCallFnId(t.value.STR_VAL);
            analyseAssignRightId(t.value.STR_VAL);

            lex_token lastToken = t;
            getToken();
            stash = lastToken;

            return rule_FN_OR_EXP();
        }
        else if (t.type == token_CONST
            || t.type == token_CONST_WHOLE_NUMBER
            || t.type == token_CONST_DEC_NUMBER
            || t.type == token_CONST_SCIENTIFIC_NOTATION
            || t.type == token_PARENTHESES_L
            || t.type == token_TYPE_STRING_LINE
            || t.type == token_NIL) {
            RLOG("<var_assignment> -> = const <expression>\n");
            return rule_EXPRESSION();
        }
    }
    return false;
}

bool rule_VAL_ASSIGNMENT() {
    // <val_assigment> -> = id <fn_or_exp>
    if (t.type == token_ASSIGN) {
        getToken();

        if (t.type == token_ID) {
            RLOG("<val_assignment> -> = id <fn_or_exp>\n");
            analyseAssignRightId(t.value.STR_VAL);
            analyseCallFnId(t.value.STR_VAL);
            lex_token lastToken = t;
            getToken();
            stash = lastToken;
            LEX_ERR_CHECK();

            return rule_FN_OR_EXP();
        }
        else if (t.type == token_CONST
            || t.type == token_CONST_WHOLE_NUMBER
            || t.type == token_CONST_DEC_NUMBER
            || t.type == token_CONST_SCIENTIFIC_NOTATION
            || t.type == token_PARENTHESES_L
            || t.type == token_TYPE_STRING_LINE
            || t.type == token_NIL) {
            RLOG("<val_assignment> -> = const <expression>\n");
            return rule_EXPRESSION();
        }
    }
    // <val_assigment> -> EPSILON
    else if (EOL_flag) {
        analyseAssignEnd();
        RLOG("<val_assignment> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_FN_OR_EXP() {
    // <fn_or_exp> -> id/const
    if (EOL_flag || t.type == token_EOF) {
        RLOG("<fn_or_exp> -> id\n");
        analyseAssignIdType();
        analyseAssignEnd();

        analyseReassignIdType();
        analyseReassignEnd();
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
        getToken();

        if (rule_INPUT_PARAM_LIST() == false) {
            return false;
        }

        Type returnedType = analyseCallEnd();

        analyseAssignType(returnedType);
        analyseAssignEnd();

        analyseReassignType(returnedType);
        analyseReassignEnd();

        if (t.type != token_PARENTHESES_R) {
            return false;
        }
        getToken();
        LEX_ERR_CHECK();
        return true;
    }
    return false;
}

bool rule_AFTER_ID() {
    // <after_id> -> = id <fn_or_exp> 
    if (t.type == token_ASSIGN) {
        getToken();


        if (t.type == token_ID) {
            RLOG("<after_id> -> = id <fn_or_exp>\n");
            analyseReassignRightId(t.value.STR_VAL);
            analyseCallFnId(t.value.STR_VAL);
            lex_token lastToken = t;
            getToken();
            stash = lastToken;
            LEX_ERR_CHECK();
            return rule_FN_OR_EXP();
        }
        else if (t.type == token_CONST
            || t.type == token_CONST_WHOLE_NUMBER
            || t.type == token_CONST_DEC_NUMBER
            || t.type == token_CONST_SCIENTIFIC_NOTATION
            || t.type == token_PARENTHESES_L
            || t.type == token_TYPE_STRING_LINE
            || t.type == token_NIL) {
            RLOG("<after_id> -> = const <expression>\n");
            return rule_EXPRESSION();
        }
    }
    // <after_id> -> ( <input_param_list> ) 
    else if (t.type == token_PARENTHESES_L) {
        RLOG("<after_id> -> ( <input_param_list> )\n");
        getToken();
        analyseReassignAbort();
        if (rule_INPUT_PARAM_LIST() == false) {
            return false;
        }
        if (t.type != token_PARENTHESES_R) {
            return false;
        }
        analyseCallEnd();
        getToken();
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
        || t.type == token_TYPE_STRING_LINE
        || t.type == token_NIL) {
        RLOG("<input_param_list> -> <input_param> <input_param_next>\n");
        return rule_INPUT_PARAM() && rule_INPUT_PARAM_NEXT();
    }
    return false;
}

bool rule_INPUT_PARAM_NEXT() {
    // <input_param_next> -> , <input_param> <input_param_next>
    if (t.type == token_COMMA) {
        RLOG("<input_param_next> -> , <input_param> <input_param_next>\n");
        getToken();

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
        || t.type == token_TYPE_STRING_LINE
        || t.type == token_NIL) {

        RLOG("<input_param> -> const\n");
        analyseCallConst(t);
        getToken();
        return true;
    }
    // <input_param> -> id <with_name>
    else if (t.type == token_ID) {
        RLOG("<input_param> -> id <with_name>\n");
        (analyseCallIdOrLabel(t.value.STR_VAL));
        getToken();

        return rule_WITH_NAME();
    }
    return false;
}

bool rule_WITH_NAME() {
    // <with_name> -> EPSILON 
    if (t.type == token_COMMA ||
        t.type == token_PARENTHESES_R) {
        RLOG("<with_name> -> EPSILON\n");
        analyseCallEpsAfterId();
        return true;
    }
    // <with_name> -> : <id_or_const>
    else if (t.type == token_COLON) {
        RLOG("<with_name> -> : <id_or_const>\n");
        getToken();

        return rule_ID_OR_CONST();
    }
    return false;
}

bool rule_ID_OR_CONST() {
    // <id_or_const> -> id
    if (t.type == token_ID) {
        RLOG("<id_or_const> -> id\n");
        LEX_ERR_CHECK();
        analyseCallIdAfterLabel(t.value.STR_VAL);
        getToken();

        return true;
    }
    // <id_or_const> -> const
    else if (t.type == token_CONST
        || t.type == token_CONST_WHOLE_NUMBER
        || t.type == token_CONST_DEC_NUMBER
        || t.type == token_CONST_SCIENTIFIC_NOTATION
        || t.type == token_TYPE_STRING_LINE
        || t.type == token_NIL) {
        RLOG("<id_or_const> -> const\n");
        analyseCallConstAfterLabel(t);
        getToken();

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
        getToken();

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
    if (rule_ID_OR_UNDERSCORE() == false) {
        return false;
    }
    if (t.type != token_ID) {
        return false;
    }
    analyseFunctionParamName(t.value.STR_VAL);
    getToken();
    LEX_ERR_CHECK();
    consume_optional_EOL();
    if (t.type != token_COLON) {
        return false;
    }
    getToken();
    LEX_ERR_CHECK();
    consume_optional_EOL();
    (analyseFunctionParamType(t.type));
    return rule_TYPE();
}

bool rule_ID_OR_UNDERSCORE() {
    // <id_or_underscore> -> id
    if (t.type == token_ID) {
        RLOG("<id_or_underscore> -> id\n");
        (analyseFunctionParamLabel(t.value.STR_VAL));
        getToken();

        return true;
    }
    // <id_or_underscore> -> _
    else if (t.type == token_UNDERSCORE) {
        RLOG("<id_or_underscore> -> _\n");
        analyseFunctionParamLabel("_");
        getToken();

        return true;
    }
    return false;
}

bool rule_RETURN_TYPE() {
    // <return_type> -> ". type"
    if (t.type == token_ARROW) {
        RLOG("<return_type> -> -> <type>\n");
        getToken();
        (analyseFunctionType(t.type));

        if (rule_TYPE()) {
            return true;
        }
    }
    // <return_type> -> EPSILON
    else if (t.type == token_BRACKET_L) {
        (analyseFunctionType(token_EMPTY));
        RLOG("<return_type> -> EPSILON");
        return true;
    }
    return false;
}

bool rule_FUNC_STAT_LIST() {
    // <func_stat_list> -> <func_stat> EOL <func_stat_list>
    if (t.type == token_LET ||
        t.type == token_VAR ||
        t.type == token_IF ||
        t.type == token_WHILE ||
        t.type == token_ID ||
        t.type == token_RETURN) {
        RLOG("<func_stat_list> -> <func_stat> EOL <func_stat_list>\n");
        if (rule_FUNC_STAT()) {
            if (EOL_flag) {
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
    prepareStatement();

    switch (t.type) {
    case token_LET:
    case token_VAR:
        RLOG("<func_stat> -> <let_or_var> <var_assignment>\n");
        analyseAssignBegin();
        return rule_LET_OR_VAR() && rule_VAR_ASSIGNMENT();
    case token_ID:
        RLOG("<func_stat> -> id <after_id>\n");
        analyseReassignStart();
        analyseReassignId(t.value.STR_VAL);
        analyseCallFnId(t.value.STR_VAL);
        getToken();

        return rule_AFTER_ID();
    case token_RETURN:
        // <func_stat> -> <return_stat>
        RLOG("<func_stat> -> <return_stat>\n");
        return rule_RETURN_STAT();
    case token_IF:
        RLOG("<func_stat> -> if <condition> { <func_stat_list> } else { <func_stat_list> }\n");
        getToken();

        analyseIfLetBegin();

        if (rule_CONDITION() == false) {
            return false;
        }

        genIfBegin();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();

        symtableStackPush(global_symtableStack, false);
        pushIfLet();
        genIfBlock();
        if (rule_BRACK_STAT_LIST() == false) {
            return false;
        }
        symtableStackPop(global_symtableStack);

        if (t.type != token_BRACKET_R) {
            return false;
        }
        getToken();

        if (t.type != token_ELSE) {
            return false;
        }
        getToken();

        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();

        symtableStackPush(global_symtableStack, false);
        genIfElse();
        if (rule_BRACK_STAT_LIST() == false) {
            return false;
        }
        symtableStackPop(global_symtableStack);
        genIfEnd();

        if (t.type != token_BRACKET_R) {
            return false;
        }
        getToken();

        return true;

    case token_WHILE:
        RLOG("<func_stat> -> while <expression> { <func_stat_list> }\n");
        getToken();

        genWhileBegin();
        if (rule_EXPRESSION() == false) {
            return false;
        }
        analyseCondition();

        consume_optional_EOL();
        if (t.type != token_BRACKET_L) {
            return false;
        }
        getToken();

        symtableStackPush(global_symtableStack, false);
        genWhileStats();
        if (rule_BRACK_STAT_LIST() == false) {
            return false;
        }
        genWhileEnd();
        symtableStackPop(global_symtableStack);

        if (t.type != token_BRACKET_R) {
            return false;
        }

        getToken();
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
        getToken();
        if (rule_RET_VAL()) {
            if (EOL_flag) {
                return rule_FUNC_STAT_LIST();
            }
        }
    }
    // <return_stat> -> EPSILON
    else if (EOL_flag) {
        RLOG("<return_stat> -> EPSILON\n");
        return true;
    }
    return false;
}

bool rule_RET_VAL() {
    analyseReturnBegin();

    // <ret_val> -> <expression>
    if (t.type == token_ID
        || t.type == token_PARENTHESES_L
        || t.type == token_CONST
        || t.type == token_CONST_WHOLE_NUMBER
        || t.type == token_CONST_DEC_NUMBER
        || t.type == token_CONST_SCIENTIFIC_NOTATION
        || t.type == token_TYPE_STRING_LINE
        || t.type == token_NIL) {
        RLOG("<ret_val> -> <expression>\n");
        return rule_EXPRESSION();
    }
    // <ret_value> -> EPSILON
    else if (EOL_flag) {
        RLOG("<ret_val> -> EPSILON\n");
        analyseReturn((Type) { 'v', false });
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
        || t.type == token_TYPE_STRING_LINE
        || t.type == token_NIL) {
        RLOG("<condition> -> <expression>\n");
        if (!rule_EXPRESSION()) {
            return false;
        }
        analyseCondition();
        return true;
    }
    // <condtion> -> let id
    if (t.type == token_LET) {
        RLOG("<condition> -> let id\n");
        getToken();

        if (t.type == token_ID) {
            analyseIfLet(t.value.STR_VAL);
            getToken();
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
        analyseAssignHint(t.type);
        getToken();
        LEX_ERR_CHECK();
        return true;
    }
    return false;
}












