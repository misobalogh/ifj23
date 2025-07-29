```
1. <program> -> <stat_list> EOF
```

```
2. <stat_list> -> <statement> EOL <stat_list>
3. <stat_list> -> EPSILON
```

```
4. <statement> -> <let_or_var> <var_assignment>
5. <statement> -> id <after_id>
6. <statement> -> func id ( <param_list> ) <return_type> { <func_stat_list> }
7. <statement> -> if <condition> { <brack_stat_list> } else { <brack_stat_list> }
8. <statement> -> while <expression> { <brack_stat_list> }
9. <statement> -> EPSILON
```

```
10. <brack_stat_list> -> <statement> EOL <brack_stat_list>
11. <brack_stat_list> -> EPSILON
```

```
12. <brack_statement> -> <let_or_var> <var_assignment>
13. <brack_statement> -> id <after_id>
14. <brack_statement> -> if <condition> { <brack_stat_list> } else { <brack_stat_list> }
15. <brack_statement> -> while <expression> { <brack_stat_list> }
16. <brack_statement> -> EPSILON
```

```
17. <let_or_var> -> let id
18. <let_or_var> -> var id
```

```
19. <var_assignment> -> : type <val_assigment>
20. <var_assignment> -> = id <fn_or_exp>
21. <var_assignment> -> = const <expression>
```

```
22. <val_assignment> -> = id <fn_or_exp>
23. <var_assignment> -> = const <expression>
24. <val_assignment> -> EPSILON
```

```
25. <fn_or_exp> -> <expression>
26. <fn_or_exp> -> ( <input_param_list> )
```

```
27. <after_id> -> = id <fn_or_exp>
28. <after_id> -> = const <expression>
29. <after_id> -> ( <input_param_list> )
```

```
30. <input_param_list> -> <input_param> <input_param_next>
31. <input_param_list> -> EPSILON
```

```
32. <input_param_next> -> , <input_param> <input_param_next>
33. <input_param_next> -> EPSILON
```

```
34. <input_param> -> id <with_name>
35. <input_param> -> const
```

```
36. <with_name> -> : <id_or_const>
37. <with_name> -> EPSILON
```

```
38. <id_or_const> -> id
39. <id_or_const> -> const
```

```
40. <param_list> -> <param> <param_next>
41. <param_list> -> EPSILON
```

```
42. <param> -> <id_or_underscore> id : <type>
```

```
43. <id_or_underscore> -> _
44. <id_or_underscore> -> id
```

```
45. <param_next> -> , <param> <param_next>
46. <param_next> -> EPSILON
```

```
47. <return_type> => -> <type>
48. <return_type> => EPSILON
```

```
49. <func_stat_list> -> <func_stat> EOL <func_stat_list>
50. <func_stat_list> -> EPSILON
```

```
51. <func_stat> -> <let_or_var> <var_assignment>
52. <func_stat> -> id <after_id>
53. <func_stat> -> return <ret_val> EOL <func_stat_list>
54. <func_stat> -> if <condition> { <func_stat_list> } else { <func_stat_list> }
55. <func_stat> -> while <expression> { <func_stat_list> }
56. <func_stat> -> EPSILON
```

```
57. <ret_val> -> <expression>
58. <ret_val> -> EPSILON
```

```
59. <condition> -> <expression>
60. <condtion> -> let id
```

```
62. <type> -> Int
63. <type> -> Int?
64. <type> -> Double
65. <type> -> Double?
66. <type> -> String
67. <type> -> String?
```

```
68. <expression> -> id
69. <expression> -> const
70. <expression> -> ( <expression> )
71. <expression> -> <expression> arithm_op <expression>
72. <expression> -> <expression> ?? <expression>
73. <expression> -> <expression>!  (force unwrap)
74. <expression> -> <expression> rel_op <expression>  
```
