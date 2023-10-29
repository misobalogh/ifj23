

// enum of IFJ23 keywords
typedef enum Keyword
{
    KEYWORD_DOUBLE,
    KEYWORD_ELSE,
    KEYWORD_FUNC,
    KEYWORD_IF,
    KEYWORD_INT,
    KEYWORD_LET,
    KEYWORD_NIL,
    KEYWORD_RETURN,
    KEYWORD_STRING,
    KEYWORD_VAR,
    KEYWORD_WHILE,
} keyword;

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
    STATE_INT,
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
    STATE_I_Int,
    STATE_N_Int,
    STATE_Int,
    STATE_Int ?,
    STATE_D_Double,
    STATE_O_Double,
    STATE_U_Double,
    STATE_B_Double,
    STATE_L_Double,
    STATE_Double,
    STATE_DOUBLE ?,
    STATE_S_Sring,
    STATE_T_Sring,
    STATE_R_Sring,
    STATE_I_Sring,
    STATE_N_Sring,
    STATE_Sring,
    STATE_Sring ?, } states;

typedef enum Type_of_token
{
    TYPE_ERROR,
    TYPE_EOF,
    TYPE_EOL,
    TYPE_ADD,
    TYPE_SUB,
    TYPE_DIV,
    TYPE_MUL,
    TYPE_OP_GREATER_THAN,
    TYPE_OP_GREATER_OR_EQUAL_THAN,
    TYPE_OP_LESS_THAN,
    TYPE_OP_LESS_OR_EQUAL_THAN,
    TYPE_OPTIONAL,
    TYPE_DEFAULT_VALUE,
    TYPE_SEMICOLON,
    TYPE_LEFT_BRACKET,
    TYPE_RIGHT_BRACKET,
    TYPE_WHOLE_NUMBER,
    TYPE_EXP_DECIMAL,
    TYPE_DEC_NUMBER,
    TYPE_EXP,
    TYPE_EXP_OPERATOR,
    TYPE_EXP_POWER,
    TYPE_NOT,
    TYPE_NOT_EQUAL,
    TYPE_EQUAL,
    TYPE_ASSING,
    TYPE_IDENTIFIER,
} type_of_token;

typedef union Value_of_token
{
    int INT_VAL;
    float FLOAT_VAL;
    char *STR_VAL;
} value_of_token;

typedef struct Dynamic_String
{
    char *data;            // current cuntent
    unsigned int size;     // current size
    unsigned int capacity; // max capacity
} dynamic_string;

typedef struct Token
{
    value_of_token token_value;
    type_of_token token_type;
    int line;
} lex_token;

// function to get new token
lex_token get_next_token();

// current token
extern lex_token current_lex_token;

// current state
extern states current_lex_state;