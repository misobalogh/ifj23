#include <stdio.h>
#include "scanner.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// function for picking a keyword from input string
type_of_token keyword_check(char *str)
{
    if (strcmp(str, "Double") == 0)
    {
        return KEYWORD_DOUBLE;
    }
    else if (strcmp(str, "else") == 0)
    {
        return KEYWORD_ELSE;
    }
    else if (strcmp(str, "func") == 0)
    {
        return KEYWORD_FUNC;
    }
    else if (strcmp(str, "if") == 0)
    {
        return KEYWORD_IF;
    }
    else if (strcmp(str, "let") == 0)
    {
        return KEYWORD_LET;
    }
    else if (strcmp(str, "nil") == 0)
    {
        return KEYWORD_NIL;
    }
    else if (strcmp(str, "return") == 0)
    {
        return KEYWORD_RETURN;
    }
    else if (strcmp(str, "var") == 0)
    {
        return KEYWORD_VAR;
    }
    else if (strcmp(str, "while") == 0)
    {
        return KEYWORD_WHILE;
    }
    else
    {
        return TYPE_IDENTIFIER;
    }
}

// function initializes dynamic string for reading nubers,string and identifiers
dynamic_string string_init(dynamic_string *str)
{
    str->size = 0;      // initial current size
    str->capacity = 32; // initial capacity of string
    str->data = (char *)malloc(str->capacity * sizeof(char));

    if (str->data == NULL)
    {
        printf("Malloc of dynamic string failed\n");
        // to do return error
    }
    return *str;
}
// function clears initialized string

void string_clear(dynamic_string *str)
{
    if (str != NULL)
    {
        free(str->data);
    }
}

// if dynamic string is full, function will realloc its memory to 2 times its previous size
dynamic_string char_insert(dynamic_string *str, char c)
{
    if (str->size == str->capacity)
    {
        str->capacity = str->capacity * 2; // doubles the maximum capacity
        str->data = (char *)realloc(str->data, str->capacity);
        if (str->data == NULL)
        {
            printf("Realloc of dynamic string failed\n");
            // to do return error
        }
    }
    str->data[str->size] = c;
    str->size += 1;
    return *str;
}

lex_token current_lex_token = {.token_type = TYPE_ERROR, .token_value = 0};

lex_token get_next_token()
{
    states current_lex_state = STATE_START;
    dynamic_string str;

    int c = 0;
    while (1)
    {
        c = getc(stdin);
        switch (current_lex_state)
        {
        case STATE_START:

            if (c == ' ')
            {
                continue;
            }
            else if (c == '/')
            {
                current_lex_state = STATE_SLASH;
            }
            else if (c == '>')
            {
                current_lex_state = STATE_GREATER_THAN;
            }
            else if (c == '<')
            {
                current_lex_state = STATE_LESS_THAN;
            }
            else if (c == EOF)
            {
                current_lex_state = STATE_EOF;
            }
            else if (c == '+')
            {
                current_lex_state = STATE_ADDITION;
            }
            else if (c == '-')
            {
                current_lex_state = STATE_SUBTRACTION;
            }
            else if (c == ':')
            {
                current_lex_state = STATE_COLON;
            }
            else if (c == '*')
            {
                current_lex_state = STATE_MULTIPLICATION;
            }
            else if (c == '?')
            {
                current_lex_state = STATE_OPTIONAL;
            }
            else if (c == ';')
            {
                current_lex_state = STATE_SEMICOLON;
            }
            else if (c == '(')
            {
                current_lex_state = STATE_LEFT_BRACKET;
            }
            else if (c == ')')
            {
                current_lex_state = STATE_RIGHT_BRACKET;
            }
            else if (c == '[')
            {
                current_lex_state = STATE_LEFT_SQUARE_BRACKET;
            }
            else if (c == ']')
            {
                current_lex_state = STATE_RIGHT_SQUARE_BRACKET;
            }
            else if (c == '{')
            {
                current_lex_state = STATE_LEFT_CURV_BRACKET;
            }
            else if (c == '}')
            {
                current_lex_state = STATE_RIGHT_CURV_BRACKET;
            }
            else if (c == '\n')
            {
                current_lex_state = STATE_EOL;
            }
            else if (c == '!')
            {
                current_lex_state = STATE_NOT;
            }
            else if (c == '=')
            {
                current_lex_state = STATE_ASSING;
            }
            else if (c == '"')
            {
                current_lex_state = STATE_Q_MARK;
                string_init(&str);
            }
            else if (c == ',')
            {
                current_lex_state = STATE_COMMA;
            }
            else if (isdigit(c))
            {
                current_lex_state = STATE_WHOLE_NUMBER;
                string_init(&str);
                char_insert(&str, c);
            }
            else if (c == '_')
            {
                current_lex_state = STATE_UNDERSCORE_IDENTIFIER;
                string_init(&str);
                char_insert(&str, c);
            }
            else if (c == 'I')
            {
                current_lex_state = STATE_I_Int;
                string_init(&str);
                char_insert(&str, c);
            }
            else if (c == 'D')
            {
                current_lex_state = STATE_D_Double;
                string_init(&str);
                char_insert(&str, c);
            }
            else if (c == 'S')
            {
                current_lex_state = STATE_S_String;
                string_init(&str);
                char_insert(&str, c);
            }
            else if (((isalpha(c)) && ((c != 'I') || (c != 'D') || (c != 'S'))))
            {
                current_lex_state = STATE_IDENTIFIER;
                string_init(&str);
                char_insert(&str, c);
            }

            else
            {
                fprintf(stderr, "ERROR: unexpected character encountered: %c\n", c);
                return (lex_token){.token_type = TYPE_ERROR, .token_value = 1};
            }

            break;

            /*
                Comments
            */

        case STATE_SLASH:
            if (c == '/')
            {
                current_lex_state = STATE_ROWCOMMENT;
            }
            else if (c == '*')
            {
                current_lex_state = STATE_BLOCKCOMMENT;
            }
            else if (isspace(c))
            {
                current_lex_state = STATE_DIVISION;
            }
            else
            {
                fprintf(stderr, "ERROR: unexpected character encountered: %c\n", c);
                return (lex_token){.token_type = TYPE_ERROR, .token_value = 1};
            }
            break;

        case STATE_ROWCOMMENT:
            if (c == '\n')
            {
                printf("ukoncil se radkovy komentar\n");
                current_lex_state = STATE_START;
            }
            break;

        case STATE_BLOCKCOMMENT:

            if (c == '*')
            {
                current_lex_state = STATE_BLOCKCOMMENT_STAR;
            }
            break;

        case STATE_BLOCKCOMMENT_STAR:
            if (c == '*')
            {
                current_lex_state = current_lex_state;
            }
            if (c == '/')
            {
                printf("ukoncil se blokovy komentar\n");
                current_lex_state = STATE_START;
            }
            else
            {
                current_lex_state = STATE_BLOCKCOMMENT;
            }
            break;

            /*
                Operators
            */

        case STATE_GREATER_THAN:
            if (c == '=')
            {
                current_lex_state = STATE_GREATER_OR_EQUAL_THAN;
            }
            else
            {
                current_lex_token.token_type = TYPE_OP_GREATER_THAN;
                ungetc(c, stdin);
                return current_lex_token;
            }

            break;

        case STATE_GREATER_OR_EQUAL_THAN:
            current_lex_token.token_type = TYPE_OP_GREATER_OR_EQUAL_THAN;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_LESS_THAN:
            if (c == '=')
            {
                current_lex_state = STATE_LESS_OR_EQUAL_THAN;
            }
            else if (c == '-')
            {
                current_lex_state = STATE_LEFT_ARROW;
            }
            else
            {
                current_lex_token.token_type = TYPE_OP_LESS_THAN;
                ungetc(c, stdin);
                return current_lex_token;
            }
            break;

        case STATE_LEFT_ARROW:
            current_lex_token.token_type = TYPE_LEFT_ARROW;
            ungetc(c, stdin);
            return current_lex_token;

        case STATE_LESS_OR_EQUAL_THAN:
            current_lex_token.token_type = TYPE_OP_LESS_OR_EQUAL_THAN;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_ADDITION:
            current_lex_token.token_type = TYPE_ADD;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_SUBTRACTION:
            if (c == '>')
            {
                current_lex_state = STATE_RIGHT_ARROW;
            }
            else
            {
                current_lex_token.token_type = TYPE_SUB;
                ungetc(c, stdin);
                return current_lex_token;
            }

            break;

        case STATE_RIGHT_ARROW:
            current_lex_token.token_type = TYPE_RIGHT_ARROW;
            ungetc(c, stdin);
            return current_lex_token;
            break;

        case STATE_MULTIPLICATION:
            current_lex_token.token_type = TYPE_MUL;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_COLON:
            current_lex_token.token_type = TYPE_COLON;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_DIVISION:
            current_lex_token.token_type = TYPE_DIV;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_OPTIONAL:
            if (c == '?')
            {
                current_lex_state = STATE_DEFAULT_VALUE;
            }
            else
            {
                current_lex_token.token_type = TYPE_OPTIONAL;
                ungetc(c, stdin);
                return current_lex_token;
            }

            break;

        case STATE_DEFAULT_VALUE:
            current_lex_token.token_type = TYPE_DEFAULT_VALUE;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_EOF:

            current_lex_token.token_type = TYPE_EOF;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_SEMICOLON:
            current_lex_token.token_type = TYPE_SEMICOLON;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_COMMA:
            current_lex_token.token_type = TYPE_COMMA;
            ungetc(c, stdin);
            return current_lex_token;

            break;
        case STATE_LEFT_BRACKET:
            current_lex_token.token_type = TYPE_LEFT_BRACKET;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_LEFT_SQUARE_BRACKET:
            current_lex_token.token_type = TYPE_LEFT_SQUARE_BRACKET;
            ungetc(c, stdin);
            return current_lex_token;
            break;

        case STATE_RIGHT_SQUARE_BRACKET:
            current_lex_token.token_type = TYPE_RIGHT_SQUARE_BRACKET;
            ungetc(c, stdin);
            return current_lex_token;
            break;

        case STATE_LEFT_CURV_BRACKET:
            current_lex_token.token_type = TYPE_LEFT_CURV_BRACKET;
            ungetc(c, stdin);
            return current_lex_token;
            break;

        case STATE_RIGHT_CURV_BRACKET:
            current_lex_token.token_type = TYPE_RIGHT_CURV_BRACKET;
            ungetc(c, stdin);
            return current_lex_token;
            break;

        case STATE_RIGHT_BRACKET:
            current_lex_token.token_type = TYPE_RIGHT_BRACKET;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_EOL:
            current_lex_token.token_type = TYPE_EOL;
            ungetc(c, stdin);
            return current_lex_token;
            break;

        case STATE_NOT:
            if (c == '=')
            {
                current_lex_state = STATE_NOT_EQUAL;
            }
            else
            {
                current_lex_token.token_type = TYPE_NOT;
                ungetc(c, stdin);
                return current_lex_token;
            }
            break;

        case STATE_NOT_EQUAL:
            current_lex_token.token_type = TYPE_NOT_EQUAL;
            return current_lex_token;
            break;

        case STATE_ASSING:
            if (c == '=')
            {
                current_lex_state = STATE_EQUAL;
            }
            else
            {
                current_lex_token.token_type = TYPE_ASSING;
                ungetc(c, stdin);
                return current_lex_token;
            }
            break;

        case STATE_EQUAL:
            current_lex_token.token_type = TYPE_EQUAL;
            ungetc(c, stdin);
            return current_lex_token;

            break;

            /*
                Numbers:
            */

        case STATE_WHOLE_NUMBER:
            if (isdigit(c))
            {
                char_insert(&str, c);
            }
            else if (c == '.')
            {
                char_insert(&str, c);
                current_lex_state = STATE_EXP_DECIMAL;
            }
            else if (c == 'e' || c == 'E')
            {
                char_insert(&str, c);
                current_lex_state = STATE_EXP;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.token_type = TYPE_WHOLE_NUMBER;

                current_lex_token.token_value.INT_VAL = atoi(str.data);
                string_clear(&str);
                return current_lex_token;
            }
            break;

        case STATE_EXP_DECIMAL:
            if (isdigit(c))
            {
                char_insert(&str, c);
                current_lex_state = STATE_DEC_NUMBER;
            }
            else
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }

            break;

        case STATE_DEC_NUMBER:
            if (isdigit(c))
            {
                char_insert(&str, c);
            }
            else if (c == 'e' || c == 'E')
            {
                char_insert(&str, c);
                current_lex_state = STATE_EXP;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.token_type = TYPE_DEC_NUMBER;
                current_lex_token.token_value.FLOAT_VAL = atof(str.data);
                string_clear(&str);
                return current_lex_token;
            }

            break;

        case STATE_EXP:
            if (isdigit(c))
            {
                char_insert(&str, c);
                current_lex_state = STATE_EXP_POWER;
            }
            else if (c == '+' || c == '-')
            {
                char_insert(&str, c);
                current_lex_state = STATE_EXP_OPERATOR;
            }
            else
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }

            break;

        case STATE_EXP_OPERATOR:
            if (isdigit(c))
            {
                char_insert(&str, c);
                current_lex_state = STATE_EXP_POWER;
            }
            else
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }

            break;

        case STATE_EXP_POWER:
            if (isdigit(c))
            {
                char_insert(&str, c);
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.token_type = TYPE_EXP_POWER;
                current_lex_token.token_value.FLOAT_VAL = atof(str.data);
                string_clear(&str);
                return current_lex_token;
            }

            break;

        case STATE_UNDERSCORE_IDENTIFIER:
            if ((isalpha(c)) || (c == '_') || (isdigit(c)))
            {
                current_lex_state = STATE_IDENTIFIER;
                char_insert(&str, c);
            }
            else
            {
                current_lex_token.token_type = TYPE_UNDERSCORE;
                ungetc(c, stdin);
                return current_lex_token;
            }
            break;

            /*
                Strings:
            */

        case STATE_IDENTIFIER:
            if ((isalpha(c)) || (isdigit(c)) || (c == '_'))
            {
                char_insert(&str, c);
            }
            else
            {
                ungetc(c, stdin);
                // current_lex_token.token_type = TYPE_IDENTIFIER;
                current_lex_token.token_type = keyword_check(str.data);
                current_lex_token.token_value.STR_VAL = str.data;
                //  string_clear(&str);
                return current_lex_token;
            }
            break;

        case STATE_I_Int:
            if (c == 'n')
            {
                current_lex_state = STATE_N_Int;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;
        case STATE_N_Int:
            if (c == 't')
            {
                current_lex_state = STATE_Int;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_Int:
            if (c == '?')
            {
                char_insert(&str, c);
                current_lex_state = STATE_Int_Q;
            }
            else if ((isalpha(c)) || (isdigit(c)) || (c == '_'))
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.token_type = KEYWORD_INT;
                return current_lex_token;
            }
            break;

        case STATE_Int_Q:
            if (c == '?')
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.token_type = KEYWORD_INT_Q;
                return current_lex_token;
            }
            break;

        case STATE_D_Double:
            if (c == 'o')
            {
                current_lex_state = STATE_O_Double;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_O_Double:
            if (c == 'u')
            {
                current_lex_state = STATE_U_Double;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_U_Double:
            if (c == 'b')
            {
                current_lex_state = STATE_B_Double;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_B_Double:
            if (c == 'l')
            {
                current_lex_state = STATE_L_Double;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_L_Double:
            if (c == 'e')
            {
                current_lex_state = STATE_Double;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_Double:
            if (c == '?')
            {
                char_insert(&str, c);
                current_lex_state = STATE_DOUBLE_Q;
            }
            else if ((isalpha(c)) || (isdigit(c)) || (c == '_'))
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.token_type = KEYWORD_DOUBLE;
                return current_lex_token;
            }
            break;

        case STATE_DOUBLE_Q:
            if (c == '?')
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.token_type = KEYWORD_DOUBLE_Q;
                return current_lex_token;
            }
            break;

        case STATE_S_String:
            if (c == 't')
            {
                current_lex_state = STATE_T_String;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_T_String:
            if (c == 'r')
            {
                current_lex_state = STATE_R_String;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_R_String:
            if (c == 'i')
            {
                current_lex_state = STATE_I_String;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_I_String:
            if (c == 'n')
            {
                current_lex_state = STATE_N_String;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_N_String:
            if (c == 'g')
            {
                current_lex_state = STATE_String;
                char_insert(&str, c);
            }
            else
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            break;

        case STATE_String:
            if (c == '?')
            {
                char_insert(&str, c);
                current_lex_state = STATE_String_Q;
            }
            else if ((isalpha(c)) || (isdigit(c)) || (c == '_'))
            {
                char_insert(&str, c);
                current_lex_state = STATE_IDENTIFIER;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.token_type = KEYWORD_STRING;
                return current_lex_token;
            }
            break;

        case STATE_String_Q:
            if (c == '?')
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.token_type = KEYWORD_STRING_Q;
                return current_lex_token;
            }
            break;

        case STATE_Q_MARK:
            if (c == '"')
            {
                current_lex_state = STATE_STRING_DONE;
            }
            else if (c != '\n' && c != EOF)
            {
                current_lex_state = STATE_STRING_ASSEMBLING;
                char_insert(&str, c);
            }
            else
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            break;

        case STATE_STRING_ASSEMBLING:
            if (c != '"' && c != '\\')
            {
                char_insert(&str, c);
            }
            else if (c == '"')
            {
                current_lex_state = STATE_STRING_DONE;
            }
            else if (c == '\\')
            {
                current_lex_state = STATE_ESCAPE_SEQUENCE;
                char_insert(&str, c);
            }
            else // to do
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            break;

        case STATE_ESCAPE_SEQUENCE:
            if (c == '"' || c == 'n' || c == '\\' || c == 'r' || c == 't')
            {
                current_lex_state = STATE_STRING_ASSEMBLING;
                char_insert(&str, c);
            }
            else if (c == 'u')
            {
                current_lex_state = STATE_ESCAPE_U;
                char_insert(&str, c);
            }
            else
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            break;

        case STATE_ESCAPE_U:
            if (c == '{')
            {
                current_lex_state = STATE_U_LEFT_BRACKET;
                char_insert(&str, c);
            }
            else
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            break;

        case STATE_U_LEFT_BRACKET:
            if (isalpha(c) || isdigit(c))
            {
                current_lex_state = STATE_U_FIRST_NUM;
                char_insert(&str, c);
            }
            else
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            break;

        case STATE_U_FIRST_NUM:
            if (c == '}')
            {
                current_lex_state = STATE_STRING_ASSEMBLING;
                char_insert(&str, c);
            }
            else if (isalpha(c) || isdigit(c))
            {
                current_lex_state = STATE_U_SECOND_NUM;
                char_insert(&str, c);
            }
            else
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            break;

        case STATE_U_SECOND_NUM:
            if (c == '}')
            {
                current_lex_state = STATE_STRING_ASSEMBLING;
                char_insert(&str, c);
            }
            else
            {
                current_lex_token.token_type = TYPE_ERROR;
                return current_lex_token;
            }
            break;

        case STATE_STRING_DONE:

            ungetc(c, stdin);
            current_lex_token.token_type = TYPE_STRING;
            current_lex_token.token_value.STR_VAL = str.data;
            return current_lex_token;

            break;

        default:
            break;
        }
    }

    return (lex_token){.token_type = TYPE_EOF};
}
