
/**
 * @brief LL TABLE FOR SYNTAX ANALYSIS
*/

/**
 * @brief TERMS
 *
 * @param EPSILON   epsilon
 * @param EOL       EOL
 * @param EOF       EOF

 * @param ID        id
 * @param CONST

 * @param LET       let
 * @param VAR       var
 *
 * @param ASSIGN    =
 * @param COLON     :
 * @param LBRACKET  (
 * @param RBRACKET  )
 * @param COMMA     ,
 * @param LPAR      {
 * @param RPAR      }
 *
 * @param NIL       nil
 * @param NOT_NIL   ! (force_unwrap)
 * @param NIL_ALT   ?? (nil_alternative)
 *
 * @param FUNC      func
 * @param ARROW     ->
 * @param RETURN    return
 * @param IF        if
 * @param ELSE      else
 * @param WHILE
 *
 * @param PLUS      +
 * @param MINUS     -
 * @param MUL       *
 * @param DIV       /
 *
 * @param EQ        ==
 * @param NEQ       !=
 * @param LT        <
 * @param GT        >
 * @param LEQ       <=
 * @param GEQ       >=
 *
 * @param CONCAT    +  (?)
 */
typedef enum terms {
    EPSILON,
    EOL,
    EOF,
    ID,
    CONST,
    LET,
    VAR,
    ASSIGN,
    COLON,
    LBRACKET,
    RBRACKET,
    COMMA,
    LPAR,
    RPAR,
    NIL,
    NOT_NIL,
    NIL_ALT,
    FUNC,
    ARROW,
    RETURN,
    IF,
    ELSE,
    WHILE,
    PLUS,
    MINUS,
    MUL,
    DIV,
    EQ,
    NEQ,
    LT,
    GT,
    LEQ,
    GEQ,
    CONCAT,
} terms;


/**
 * @brief NONTERMINALS
 * 
 * @param program
 * @param function          
 * @param param_list        
 * @param return_type       
 * @param func_stat_list    
 * @param param             
 * @param type              
 * @param stat_list         
 * @param statement         
 * @param definition_keyword    
 * @param expression        
 * @param statement_list    
 * @param input_param_list  
 * @param input_param       
 * ////@param not_rel_expression    
 */
typedef enum nonterminals {
    program,
    function,
    param_list,
    return_type,
    func_stat_list,
    param,
    type,
    stat_list,
    statement,
    definition_keyword,
    expression,
    input_param_list,
    input_param,
    // not_rel_expression,
} nonterminals;