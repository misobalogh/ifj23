#include <stdio.h>
#include "scanner.h"
#include <ctype.h>

states current_lex_state = STATE_START;
lex_token current_lex_token = {.token_type = TYPE_ERROR, .token_value = 0};

lex_token get_next_token()
{
    char c;
    // comments and block comments
    while ((c = getc(stdin)) != EOF)
    {
        switch (current_lex_state)
        {
        case STATE_START:
            if (isspace(c))
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
                current_lex_state = STATE_GREATER_OR_EQUAL_THAN;
                current_lex_token.token_type = TYPE_OP_GREATER_THAN;
            }
            else
            {
                current_lex_token.token_type = TYPE_OP_GREATER_OR_EQUAL_THAN;
                ungetc(c, stdin);
            }
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