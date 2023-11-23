#include <stdio.h>
#include "macros.h"
#include "scanner.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "token_types.h"

dynamic_string str = { .data = NULL, .size = 0, .capacity = 0 };

lex_token current_lex_token = { .type = token_LEX_ERROR, .value = 0 };


// function for picking a keyword from input string
tokenType keyword_check(char* str)
{
    if (str == NULL)
    {
        return token_LEX_ERROR;
    }
    else if (strcmp(str, "Double") == 0)
    {
        return token_TYPE_DOUBLE;
    }
    else if (strcmp(str, "else") == 0)
    {
        return token_ELSE;
    }
    else if (strcmp(str, "func") == 0)
    {
        return token_FUNC;
    }
    else if (strcmp(str, "if") == 0)
    {
        return token_IF;
    }
    else if (strcmp(str, "let") == 0)
    {
        return token_LET;
    }
    else if (strcmp(str, "nil") == 0)
    {
        return token_NIL;
    }
    else if (strcmp(str, "return") == 0)
    {
        return token_RETURN;
    }
    else if (strcmp(str, "var") == 0)
    {
        return token_VAR;
    }
    else if (strcmp(str, "while") == 0)
    {
        return token_WHILE;
    }
    else
    {
        return token_ID;
    }
}

// function initializes dynamic string for reading nubers,string and identifiers
dynamic_string string_init(dynamic_string* str)
{
    str->size = 0;      // initial current size
    str->capacity = 32; // initial capacity of string
    str->data = (char*)malloc(str->capacity * sizeof(char));

    if (str->data == NULL)
    {
        fprintf(stderr, "Malloc of dynamic string failed\n");
        // to do return error
    }
    return *str;
}
// function clears initialized string

void string_clear(dynamic_string* str)
{
    if (str->data == NULL)
    {
        return;
    }
    free(str->data);
    str->data = NULL;
}

// if dynamic string is full, function will realloc its memory to 2 times its previous size
dynamic_string char_insert(dynamic_string* str, char c)
{
    if (str->size == str->capacity)
    {
        str->capacity = str->capacity * 2; // doubles the maximum capacity
        str->data = (char*)realloc(str->data, str->capacity);
        if (str->data == NULL)
        {
            fprintf(stderr, "Realloc of dynamic string failed\n");
            // to do return error
        }
    }
    str->data[str->size] = c;
    str->size += 1;
    return *str;
}


lex_token get_next_token()
{
    states current_lex_state = STATE_START;

    string_clear(&str);

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
                fprintf(stderr, "ERROR: unexpected character encountered: ");
                if (isprint(c)) {
                    fprintf(stderr, "%c\n", c);
                }
                else {
                    fprintf(stderr, "\\%i\n", c);
                }
                string_clear(&str);
                return (lex_token) { .type = token_LEX_ERROR, .value = 1 };
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
            else
            {
                ungetc(c, stdin);
                current_lex_state = STATE_DIVISION;
            }
            break;

        case STATE_ROWCOMMENT:
            if (c == '\n')
            {
                ungetc(c, stdin); // zmena
                // printf("ukoncil se radkovy komentar\n");
                current_lex_state = STATE_START;
            }
            else if (c == EOF)
            {
                ungetc(c, stdin);                        // zmena
                // printf("ukoncil se radkovy komentar\n"); // zmena
                current_lex_state = STATE_START;         // zmena
            }
            else
            {
                continue;
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
                // printf("ukoncil se blokovy komentar\n");
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
                current_lex_token.type = token_MORE;
                ungetc(c, stdin);
                return current_lex_token;
            }

            break;

        case STATE_GREATER_OR_EQUAL_THAN:
            current_lex_token.type = token_MORE_EQ;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_LESS_THAN:
            if (c == '=')
            {
                current_lex_state = STATE_LESS_OR_EQUAL_THAN;
            }
            else
            {
                current_lex_token.type = token_LESS;
                ungetc(c, stdin);
                return current_lex_token;
            }
            break;

        case STATE_LESS_OR_EQUAL_THAN:
            current_lex_token.type = token_LESS_EQ;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_ADDITION:
            current_lex_token.type = token_PLUS;
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
                current_lex_token.type = token_MINUS;
                ungetc(c, stdin);
                return current_lex_token;
            }

            break;

        case STATE_RIGHT_ARROW:
            current_lex_token.type = token_ARROW;
            ungetc(c, stdin);
            return current_lex_token;
            break;

        case STATE_MULTIPLICATION:
            current_lex_token.type = token_MUL;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_COLON:
            current_lex_token.type = token_COLON;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_DIVISION:
            current_lex_token.type = token_DIV;
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
                fprintf(stderr, "ERROR: unexpected character encountered: ");
                if (isprint(c)) {
                    fprintf(stderr, "%c\n", c);
                }
                else {
                    fprintf(stderr, "\\%i\n", c);
                }
                string_clear(&str);
                return (lex_token) { .type = token_LEX_ERROR, .value = 1 };
            }

            break;

        case STATE_DEFAULT_VALUE:
            current_lex_token.type = token_CONCAT;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_EOF:

            current_lex_token.type = token_EOF;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_SEMICOLON:
            current_lex_token.type = token_SEMICOLON;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_COMMA:
            current_lex_token.type = token_COMMA;
            ungetc(c, stdin);
            return current_lex_token;

            break;
        case STATE_LEFT_BRACKET:
            current_lex_token.type = token_PARENTHESES_L;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_LEFT_CURV_BRACKET:
            current_lex_token.type = token_BRACKET_L;
            ungetc(c, stdin);
            return current_lex_token;
            break;

        case STATE_RIGHT_CURV_BRACKET:
            current_lex_token.type = token_BRACKET_R;
            ungetc(c, stdin);
            return current_lex_token;
            break;

        case STATE_RIGHT_BRACKET:
            current_lex_token.type = token_PARENTHESES_R;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_EOL:
            current_lex_token.type = token_EOL;
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
                current_lex_token.type = token_FORCE_UNWRAP;
                ungetc(c, stdin);
                return current_lex_token;
            }
            break;

        case STATE_NOT_EQUAL:
            current_lex_token.type = token_NEQ;
            return current_lex_token;
            break;

        case STATE_ASSING:
            if (c == '=')
            {
                current_lex_state = STATE_EQUAL;
            }
            else
            {
                current_lex_token.type = token_ASSIGN;
                ungetc(c, stdin);
                return current_lex_token;
            }
            break;

        case STATE_EQUAL:
            current_lex_token.type = token_EQ;
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
                current_lex_token.type = token_CONST_WHOLE_NUMBER;
                char_insert(&str, '\0'); // TODO: check if this is correct
                current_lex_token.value.INT_VAL = atoi(str.data);
                // string_clear(&str);
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
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
                current_lex_token.type = token_CONST_DEC_NUMBER;
                char_insert(&str, '\0'); // TODO: check if this is correct
                current_lex_token.value.FLOAT_VAL = atof(str.data);
                // string_clear(&str);
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
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
                current_lex_token.type = token_CONST_SCIENTIFIC_NOTATION;
                current_lex_token.value.FLOAT_VAL = atof(str.data);
                // string_clear(&str);
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
                current_lex_token.type = token_UNDERSCORE;
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
                // current_lex_token.type = TYPE_IDENTIFIER;
                char_insert(&str, '\0'); // TODO: check if this is correct
                current_lex_token.type = keyword_check(str.data);

                current_lex_token.value.STR_VAL = malloc(str.size);
                CHECK_MEMORY_ALLOC(current_lex_token.value.STR_VAL);
                strncpy(current_lex_token.value.STR_VAL, str.data, str.size);

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
                current_lex_token.type = token_TYPE_INT;
                return current_lex_token;
            }
            break;

        case STATE_Int_Q:
            if (c == '?')
            {
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
                return current_lex_token;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.type = token_TYPE_INT_Q;
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
                current_lex_token.type = token_TYPE_DOUBLE;
                return current_lex_token;
            }
            break;

        case STATE_DOUBLE_Q:
            if (c == '?')
            {
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
                return current_lex_token;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.type = token_TYPE_DOUBLE_Q;
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
                current_lex_token.type = token_TYPE_STRING;
                return current_lex_token;
            }
            break;

        case STATE_String_Q:
            if (c == '?')
            {
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
                return current_lex_token;
            }
            else
            {
                ungetc(c, stdin);
                current_lex_token.type = token_TYPE_STRING_Q;
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
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
                string_clear(&str);
                current_lex_token.type = token_LEX_ERROR;
                return current_lex_token;
            }
            break;

        case STATE_STRING_DONE:

            ungetc(c, stdin);
            current_lex_token.type = token_TYPE_STRING_LINE;
            current_lex_token.value.STR_VAL = str.data;
            return current_lex_token;

            break;

        default:
            break;
        }
    }

    return (lex_token) { .type = token_EOF };
}
