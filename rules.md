
# Rules for syntactic analysis 

### program
```
1. <program> -> <stat_list> EOF
```

# Functions

### function
```
2. <function> -> func id ( <param_list> ) <return_type> { <func_stat_list> }
```

### return_type
```
3. <return_type> => -> <type>
4. <return_type> => EPSILON
```

### function_stat_list
```
5. <func_stat_list> -> <stat_list> EOL <return_stat> 
6. <func_stat_list> -> <return_stat>
7. <func_stat_list> -> if <condition> { <func_stat_list> } else { <func_stat_list> }
8. <func_stat_list> -> while <expression> { <func_stat_list> }
9. <func_stat_list> -> EPSILON
```
### condition
```
10. <condition> -> <expression>
11. <condtion>  -> let id
```
### return_stat
```
12. <return_stat> -> return <ret_value> EOL <func_stat_list>
13. <return_stat> -> EPSILON
```
### ret_value
```
14. <ret_value> -> <expression>
15. <ret_value> -> EPSILON
```


### param_list

```
16. <param_list> -> EPSILON
17. <param_list> -> <param> <param_next>
```
### param_next
```
18. <param_next> -> , <param> <param_next>
19. <param_next> -> EPSILON
```

### param
```
20. <param> -> id id : <type>
```

# Statements

### stat_list
```
21. <stat_list> -> <statement> EOL <stat_list>
22. <stat_list> -> EPSILON
```

### statement
```
23. <statement> -> <function> EOL <stat_list>
24. <statement> -> <var_definition> <var_assigment> 
25. <statement> -> id = <expression>
26. <statement> -> <expression> 
27. <statement> -> if <condition> { <stat_list> } else { <stat_list> }
28. <statement> -> while <expression> { <stat_list> }
29. <statement> -> EOL
30. <statement> -> EPSILON
```

### var_definition
```
31. <var_definition> -> let id
32. <var_definition> -> var id
```

### var_assigment
```
33. <var_assigment> -> : <type> <val_assigment>
34. <var_assigment> -> = <expression>
```

### val_assigment
```
35. <val_assigment> -> = <expression>
36. <val_assigment> -> EPSILON
```

### input_param_list
```
37. <input_param_list> -> EPSILON
38. <input_param_list> -> <input_param> <input_param_next>
```
### input_param_next
```
39. <input_param_next> -> , <input_param> <input_param_next>
40. <input_param_next> -> EPSILON
```

### input_param
```
41. <input_param> -> const        
42. <input_param> -> id <with_name>
```

### with_name
```
43. <with_name> -> EPSILON 
44. <with_name> -> : <id_or_const>
```

### id_or_const
```
45. <id_or_const> -> id
46. <id_or_const> -> const
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

52. <expression> -> id(input_param_list)

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


