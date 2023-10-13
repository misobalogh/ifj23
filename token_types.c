#include "token_types.h"
#include <stdbool.h>

bool isTerminal(tokenType type) {
    return type < token_TERMINAL;
}

bool isNonterminal(tokenType type) {
    return type > token_TERMINAL && type < token_NONTERMINAL;
}
