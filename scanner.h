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
    STATE_RIGHT_ARROW,
    STATE_LEFT_ARROW,
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
} states;

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
    TYPE_LEFT_CURV_BRACKET,
    TYPE_RIGHT_CURV_BRACKET,
    TYPE_LEFT_SQUARE_BRACKET,
    TYPE_RIGHT_SQUARE_BRACKET,
    TYPE_RIGHT_ARROW,
    TYPE_LEFT_ARROW,
    TYPE_WHOLE_NUMBER,
    TYPE_DEC_NUMBER,
    TYPE_EXP_POWER,
    TYPE_NOT,
    TYPE_NOT_EQUAL,
    TYPE_EQUAL,
    TYPE_ASSING,
    TYPE_IDENTIFIER,
    TYPE_COLON,
    TYPE_INT,
    TYPE_INT_Q,
    TYPE_DOUBLE,
    TYPE_DOUBLE_Q,
    TYPE_STRING,
    TYPE_STRING_Q,
    KEYWORD_DOUBLE,
    KEYWORD_DOUBLE_Q,
    KEYWORD_ELSE,
    KEYWORD_FUNC,
    KEYWORD_IF,
    KEYWORD_INT,
    KEYWORD_INT_Q,
    KEYWORD_LET,
    KEYWORD_NIL,
    KEYWORD_RETURN,
    KEYWORD_STRING,
    KEYWORD_STRING_Q,
    KEYWORD_VAR,
    KEYWORD_WHILE,
} type_of_token;

typedef union Value_of_token
{
    int INT_VAL;
    float FLOAT_VAL;
    char *STR_VAL;
} value_of_token;

typedef struct Token
{
    value_of_token token_value;
    type_of_token token_type;
    int line;
} lex_token;

typedef struct Dynamic_String
{
    char *data;            // current cuntent
    unsigned int size;     // current size
    unsigned int capacity; // max capacity
} dynamic_string;

// function to get new token
lex_token get_next_token();

// current token
extern lex_token current_lex_token;

// current state
extern states current_lex_state;
