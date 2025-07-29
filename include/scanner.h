/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_precedence_parser.c
* Description: Precedence syntactic analysis (parser for expression)
* Author: TADEAS ZOBAL, xzobal02
* Faculty: FIT VUT
* Date: 6.10.2023

* Comments:

***************************************************************/

// enum of FSM states
typedef enum States
{
    STATE_START,
    STATE_SLASH,
    STATE_ROWCOMMENT,
    STATE_BLOCKCOMMENT,
    STATE_BLOCKCOMMENT_STAR,
    STATE_LEFT_BRACKET,
    STATE_RIGHT_BRACKET,
    STATE_LEFT_CURV_BRACKET,
    STATE_RIGHT_CURV_BRACKET,
    STATE_LEFT_SQUARE_BRACKET,
    STATE_RIGHT_SQUARE_BRACKET,
    STATE_EQUAL,
    STATE_ASSING,
    STATE_NOT_EQUAL,
    STATE_LESS_THAN,
    STATE_LESS_OR_EQUAL_THAN,
    STATE_GREATER_THAN,
    STATE_GREATER_OR_EQUAL_THAN,
    STATE_ADDITION,
    STATE_SUBTRACTION,
    STATE_MULTIPLICATION,
    STATE_DIVISION,
    STATE_WHOLE_NUMBER,
    STATE_EXP_DECIMAL,
    STATE_EXP,          // e
    STATE_EXP_POWER,    //^10
    STATE_EXP_OPERATOR, //+-
    STATE_DEC_NUMBER,   // .(decimal number)
    STATE_SEMICOLON,
    STATE_OPTIONAL,      //?
    STATE_DEFAULT_VALUE, //??
    STATE_UNDERSCORE_IDENTIFIER,
    STATE_IDENTIFIER,
    STATE_NOT,
    STATE_EOF,
    STATE_EOL,
    STATE_RIGHT_ARROW, //->
    STATE_LEFT_ARROW,  //<-
    STATE_I_Int,
    STATE_N_Int,
    STATE_Int,
    STATE_Int_Q,
    STATE_D_Double,
    STATE_O_Double,
    STATE_U_Double,
    STATE_B_Double,
    STATE_L_Double,
    STATE_Double,
    STATE_DOUBLE_Q,
    STATE_S_String,
    STATE_T_String,
    STATE_R_String,
    STATE_I_String,
    STATE_N_String,
    STATE_String,
    STATE_String_Q,
    STATE_COLON,
    STATE_COMMA,
    STATE_Q_MARK,
    STATE_STRING_ASSEMBLING,
    STATE_ESCAPE_SEQUENCE,
    STATE_ESCAPE_U,
    STATE_U_LEFT_BRACKET,
    STATE_U_FIRST_NUM,
    STATE_U_SECOND_NUM,
    STATE_STRING_DONE,
    STATE_Q_MARK_2,
    STATE_ML_STRING_ASSEMBLING,
    STATE_ML_ESCAPE_SEQUENCE,
    STATE_ML_ESCAPE_U,
    STATE_ML_U_LEFT_BRACKET,
    STATE_ML_U_FIRST_NUM,
    STATE_ML_U_SECOND_NUM,
    STATE_Q_MARK_OUT1,
    STATE_Q_MARK_OUT2,
    STATE_NESTED_COMMENT1,
    STATE_NESTED_COMMENT2,
    STATE_NESTED_COMMENT3,
    STATE_NESTED_COMMENT4,
} states;