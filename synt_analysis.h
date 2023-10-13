// Main function
bool SyntacticAnalysis();


// Two methods - one for expressions, second for others

bool precedenceParser();
bool recursiveParser();


// private functions called by precedenceParser(); and recursiveParser();
bool parseStatement();
bool parseExpression();
bool parseFactor();
bool parseTerm();
bool parseDeclaration();
bool parseIfStatement();
bool parseWhileLoop();
bool parseFunctionCall();