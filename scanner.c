#include <stdio.h>
#include "scanner.h"
#include <ctype.h>
#include <stdlib.h>

// function initializes dynamic string for reading nubers,string and identifiers
void string_init(dynamic_string *str)
{
    str->size = 0;      // initial current size
    str->capacity = 32; // initial capacity of string
    str->data = (char *)malloc(str->capacity * sizeof(char));

    if (str->data == NULL)
    {
        printf("Malloc of dynamic string failed\n");
        return;
    }
}

// if dynamic string is full, function will realloc its memory to 2 times its previous size
void char_insert(dynamic_string *str, char c)
{
    if (str->size == str->capacity)
    {
        str->capacity = str->capacity * 2; // doubles the maximum capacity
        str->data = (char *)realloc(str->data, str->capacity);
        if (str->data == NULL)
        {
            printf("Realloc of dynamic string failed\n");
            return;
        }
    }
}

lex_token current_lex_token = {.token_type = TYPE_ERROR, .token_value = 0};

lex_token get_next_token()
{
    states current_lex_state = STATE_START;

    int c = 0;
    while (1)
    {
        c = getc(stdin);
        switch (current_lex_state)
        {
        case STATE_START:
            // if (isspace(c))
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
            else
            {
                fprintf(stderr, "ERROR: unexpected character encountered: %c\n", c);
                return (lex_token){.token_type = TYPE_ERROR, .token_value = 1};
            }

            break;

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

        case STATE_GREATER_THAN:
            if (c == '=')
            {
                // moves to STATE_GREATER_OR_EQUAL_THEN
                current_lex_state = STATE_GREATER_OR_EQUAL_THAN;
                // current_lex_state = STATE_START;
            }
            else
            {
                current_lex_token.token_type = TYPE_OP_GREATER_THAN;
                // current_lex_state = STATE_START;
                ungetc(c, stdin);
                return current_lex_token;
            }

            break;

        case STATE_GREATER_OR_EQUAL_THAN:
            current_lex_token.token_type = TYPE_OP_GREATER_OR_EQUAL_THAN;
            return current_lex_token;

            break;

        case STATE_LESS_THAN:
            if (c == '=')
            {
                current_lex_state = STATE_LESS_OR_EQUAL_THAN;
            }
            else
            {
                current_lex_token.token_type = TYPE_OP_LESS_THAN;
                ungetc(c, stdin);
                return current_lex_token;
            }
            break;

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
            current_lex_token.token_type = TYPE_SUB;
            ungetc(c, stdin);
            return current_lex_token;

            break;

        case STATE_MULTIPLICATION:
            current_lex_token.token_type = TYPE_MUL;
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
        case STATE_LEFT_BRACKET:
            current_lex_token.token_type = TYPE_LEFT_BRACKET;
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

        default:
            break;
        }
    }

    return (lex_token){.token_type = TYPE_EOF};
}

/*
int main()
{
    current_state = STATE_START;
    get_next_token();
    return 0;
}
*/