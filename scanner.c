#include <stdio.h>
#include "scanner.h"
#include <ctype.h>

token get_next_token()
{
    char c;
// comments and block comments
    while ((c = getc(stdin)) != EOF)
    {
        switch (current_state)
        {
        case STATE_START:
            if (isspace(c))
            {
                continue;
            }
            if (c == '/')
            {
                current_state = STATE_SLASH;
            }
            else
            {
                fprintf(stderr, "ERROR: unexpected character encountered: %c\n", c);
                return (token){.token_type = TYPE_ERROR, .token_value = 1};
            }
            break;

        case STATE_SLASH:
            if (c == '/')
            {
                current_state = STATE_ROWCOMMENT;
            }
            else if (c == '*')
            {
                current_state = STATE_BLOCKCOMMENT;
            }
            else
            {
                fprintf(stderr, "ERROR: unexpected character encountered: %c\n", c);
                return (token){.token_type = TYPE_ERROR, .token_value = 1};
            }
            break;

        case STATE_ROWCOMMENT:
            if (c == '\n')
            {
                printf("ukoncil se radkovy komentar\n");
                current_state = STATE_START;
            }
            break;

        case STATE_BLOCKCOMMENT:

            if (c == '*')
            {
                current_state = STATE_BLOCKCOMMENT_STAR;
            }
            break;

        case STATE_BLOCKCOMMENT_STAR:
            if (c == '*')
            {
                current_state = current_state;
            }
            if (c == '/')
            {
                printf("ukoncil se blokovy komentar\n");
                current_state = STATE_START;
            }
            else
            {
                current_state = STATE_BLOCKCOMMENT;
            }
            break;

        default:
            break;
        }
    }

    return (token){.token_type = TYPE_EOF};
}

int main()
{
    current_state = STATE_START;
    get_next_token();
    return 0;
}