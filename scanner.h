

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
    STATE_NOT_EQUAL,
    STATE_LESS_THAN,
    STATE_LESS_OR_EQUAL_THAN,
    STATE_GREATER_THAN,
    STATE_GREATER_OR_EQUAL_THAN,
    STATE_ADDITION,
    STATE_SUBTRACTION,
    STATE_MULTIPLICATION,
    STATE_DIVISION,
    STATE_NUMBER,
    STATE_STRING, // to do string
    STATE_INT,
    STATE_DOUBLE,  // to do
    STATE_KEYWORD, // to do
} states;

typedef enum Type_of_token
{
    TYPE_ERROR,
    TYPE_EOF,
    TYPE_ADD,
    TYPE_SUB,
    TYPE_DIV,
    TYPE_MUL,
    TYPE_OP_GREATER_THAN,
    TYPE_OP_GREATER_OR_EQUAL_THAN,
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

// function to get new token
lex_token get_next_token();

// current token
extern lex_token current_lex_token;

// current state
extern states current_lex_state;