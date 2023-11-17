# TODO

- [ ] Int?, Double?, String? (optional types) - how to implement?
  - add as separate types
- [ ] ?? is not concatenation, its alternative for nil (e.g. NIL_ALT)
- [ ] rename ! to NOT_NIL
- [ ] concat operator + (string + string)
- [ ] Built-in functions
- [ ] add negative numbers to expression parser (1-(-2))
- [ ] add rule for optional EOL - it **can be** nearly anywhere, but it **has to be** after certain statements (there can also be tab, space...)
- [ ] Expression pareser: E ?? E, left side maybe shouldnt be constant

# Notes

- Statements end with EOL or "{"
- if c {} **else** {} - else is always required (in basic assigment)
- foo(with: 10, and: 20) - named parameters, and values are terms, not expressions

```swift
let a : Int? = readInt()
if let a { // means if a != nil, then:
    // now here a is type of Int, not Int?
}
```

```swift
func g(x1 x: Int) -> Int { // name and id cannot be the same!
    if (x > 0) {
        let x = f(x)   /* f(x) is evaluated before x is shadowed, meaning in "f",
                         "x" is the original function parameter, */
        return x // return f(x), this one is the "let x"
    }
}
```

- when to switch to precedence parser? if i have `<expression>` in rule
- when switching to precedece parser, i need to store the token i switched on, so i can use it in the expression, (2 options - store it in the stack, or in cache, or as paramater for `precedence_parser(param)` function)
