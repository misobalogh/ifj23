
func add(a a: Int, b b: Int) -> Int {
    return a + b
}

func multiply(a a: Double, b b: Double) -> Double {
    return a * b
}

let x = 5
let y = 3
let result1 = add(a: x, b: y)

let m = 2.5
let n = 1.5
let result2 = multiply(a: m, b: n)

write(result1, " ", result2, "\n")
