
# Rules for syntactic analysis 

# Functions

### function
```
<function> -> func id ( <param_list> ) -> <return_type> { <func_stat_list> }
<function> -> func id ( <param_list> ) { <func_stat_list> }
```

### function_stat_list
```
<func_stat_list> -> <stat_list>
<func_stat_list> -> return <expression>
```


### param_list

```
<param_list> -> <param> , <param_list>
<param_list> -> <param>
<param_list> -> EPSILON
```

### param
```
<param> -> <param_name> <param_id> : <type> <param_list> 
```

# Statements
### stat_list
```
<stat_list> -> <statement> NEW_LINE <stat_list>
<stat_list> -> EPSILON
```

##  
### statement
```
1. Variable definition
<statement> -> <definition_keyword> id : type = <expression>
<statement> -> <definition_keyword> id : type
<statement> -> <definition_keyword> id = <expression>
2. Assignment
<statement> -> id = <expression>
3. If statement
<statement> -> if <expression> { <statement_list> } else { <statement_list> }
<statement> -> if let id { <statement_list> } else { <statement_list> }
<statement> -> while <expression> { <statement_list> }
<statement> -> id = function_id(input_param_list)
<statement> -> function_id(input_param_list)
```
### definition_keyword
```
<definition_keyword> -> let
<definition_keyword> -> var
```

### input_param_list
```
<input_param_list> -> <input_param>
<input_param_list> -> <input_param>, <input_param_list>
<input_param_list> -> EPSILON
```

### input_param
```
<input_param> -> <input_param_name> : <id>          (id = const/variable, not expr)
<input_param> -> <input_param_name> : CONST         (id = const/variable, not expr)
<input_param_name> => id
<input_param_name> => EPSILON
```

# Expressions
```
<expression> -> id
<expression> -> ( <expression> )
<expression> -> <expression> arithm_op <expression>
<expression> -> <expression> ?? <expression>
<expression> -> <expression>!                       (force unwrap)
<expression> -> <expression> rel_op <expression>    (not 100% correct)

something like this instead:
<expression> -> <not_rel_expression> rel_op <not_rel_expression>  
```

# Types
```
<type> -> Int
<type> -> Double
<type> -> String
```


## Terms

```ts
EOL '\n'
EOF EOF

LET let
VAR var

IF if
ELSE else
WHILE while

INT Int
DOUBLE Double 
STRING String

NIL nil

ID identifier
FUNC func

RETURN return
 
LEFT_PARENTHESIS  (
RIGHT_PARENTHESIS  )
COMMA  ,
ARROW  ->
COLON  :
```

