
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
5. <func_stat_list> -> <func_stat> EOL <func_stat_list>
6. <func_stat_list> -> EPSILON 
```
### func_stat
```
 7. <func_stat> -> <return_stat>
 8. <func_stat> -> <var_definition> <var_assigment> 
 9. <func_stat> -> id = <expression>
10. <func_stat> -> <expression> 
11. <func_stat> -> if <condition> { <func_stat_list> } else { <func_stat_list> }
12. <func_stat> -> while <expression> { <func_stat_list> }
13. <func_stat> -> EOL
14. <func_stat> -> EPSILON
```
### condition
```
15. <condition> -> <expression>
16. <condtion>  -> let id
```
### return_stat
```
17. <return_stat> -> return <ret_value> EOL <func_stat_list>
18. <return_stat> -> EPSILON
```
### ret_value
```
19. <ret_value> -> <expression>
20. <ret_value> -> EPSILON
```


### param_list

```
21. <param_list> -> EPSILON
22. <param_list> -> <param> <param_next>
```
### param_next
```
23. <param_next> -> , <param> <param_next>
24. <param_next> -> EPSILON
```

### param
```
25. <param> -> id id : <type>
```

# Statements

### stat_list
```
26. <stat_list> -> <statement> EOL <stat_list>
27. <stat_list> -> EPSILON
```

### statement
```
28. <statement> -> <function> EOL <stat_list>
29. <statement> -> <var_definition> <var_assigment> 
30. <statement> -> id = <expression>
31. <statement> -> <expression> 
32. <statement> -> if <condition> { <stat_list> } else { <stat_list> }
33. <statement> -> while <expression> { <stat_list> }
34. <statement> -> EOL
35. <statement> -> EPSILON
```

### var_definition
```
36. <var_definition> -> let id
37. <var_definition> -> var id
```

### var_assigment
```
38. <var_assigment> -> : <type> <val_assigment>
39. <var_assigment> -> = <expression>
```

### val_assigment
```
40. <val_assigment> -> = <expression>
41. <val_assigment> -> EPSILON
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


