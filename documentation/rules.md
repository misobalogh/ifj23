
# Rules for syntactic analysis 

### program
```c
1. <program> -> <stat_list> EOF
```

# Functions

### function
```c
2. <function> -> func id ( <param_list> ) <return_type> { <func_stat_list> }
```

### param_list
```c
3. <param_list> -> <param> <param_next>
4. <param_list> -> EPSILON
```

### param
```c
5. <param> -> id id : <type>
```

### param_next
```c
6. <param_next> -> , <param> <param_next>
7. <param_next> -> EPSILON
```



### return_type
```c
8. <return_type> => -> <type>
9. <return_type> => EPSILON
```

### function_stat_list
```c
10. <func_stat_list> -> <func_stat> EOL <func_stat_list>
11. <func_stat_list> -> EPSILON 
```

### func_stat
```c
12. <func_stat> -> <return_stat>
13. <func_stat> -> <let_or_var> <var_assignment>
14. <func_stat> -> id <after_id>
15. <func_stat> -> if <condition> { <func_stat_list> } else { <func_stat_list> }
16. <func_stat> -> while <expression> { <func_stat_list> }
17. <func_stat> -> EPSILON
```

### let_or_var
```c
18. <let_or_var> -> let id
19. <let_or_var> -> var id
```

### var_assignment
```c
20. <var_assignment> -> : <type> <val_assigment>
21. <var_assignment> -> = id <fn_or_exp>
```

### val_assigment
```c
22. <val_assignment> -> = id <fn_or_exp>
23. <val_assignment> -> EPSILON
```
### after_id
```c
24. <after_id> -> = id <fn_or_exp>       // assign value
25. <after_id> -> ( <input_param_list> ) // function call
```

### fn_or_exp
```c
26. <fn_or_exp> -> <expression>  // (need SECOND(expression))
27. <fn_or_exp> -> ( <input_param_list> )
```

### condition
```
28. <condition> -> <expression>
29. <condtion>  -> let id
```
### return_stat
```
30. <return_stat> -> return <ret_val> EOL <func_stat_list>
31. <return_stat> -> EPSILON
```
### ret_val
```
19. <ret_val> -> <expression>
20. <ret_val> -> EPSILON
```



# Statements

### stat_list
```
26. <stat_list> -> <statement> EOL <stat_list>
27. <stat_list> -> EPSILON // remove?
```

### statement
```
28. <statement> -> <function> EOL <stat_list>
29. <statement> -> <var_definition> <var_assignment> 
30. <statement> -> id = <expression>
31. <statement> -> <expression> 
32. <statement> -> if <condition> { <stat_list> } else { <stat_list> }
33. <statement> -> while <expression> { <stat_list> }
35. <statement> -> EPSILON
```

### input_param_list
```
42. <input_param_list> -> EPSILON
43. <input_param_list> -> <input_param> <input_param_next>
```
### input_param_next
```
44. <input_param_next> -> , <input_param> <input_param_next>
45. <input_param_next> -> EPSILON
```

### input_param
```
46. <input_param> -> const        
47. <input_param> -> id <with_name>
```

### with_name
```
48. <with_name> -> EPSILON 
49. <with_name> -> : <id_or_const>
```

### id_or_const
```
50. <id_or_const> -> id
51. <id_or_const> -> const
```





# --------------------------------------------------------------

# Precedence syntacitc analysis

# Expressions
```
45. <expression> -> id
46. <expression> -> const
47. <expression> -> ( <expression> )
48. <expression> -> <expression> arithm_op <expression>
49. <expression> -> <expression> ?? <expression>
50. <expression> -> <expression>!                       (force unwrap)
51. <expression> -> <expression> rel_op <expression>    (not 100% correct)

something like this instead:
<expression> -> <not_rel_expression> rel_op <not_rel_expression>  
```

# Types
```
53. <type> -> Int
54. <type> -> Int?
55. <type> -> Double
56. <type> -> Double?
57. <type> -> String
58. <type> -> String?
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


