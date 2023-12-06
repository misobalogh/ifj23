
func add(a innerA: Int, b innerB: Int) -> Int {
    return innerA + innerB
}

func multiply(a innerA: Double, b innerB: Double) -> Double {
    return innerA * innerB
}

let x = 5
let y = 3
let result1 = add(a: x, b: y)

let m = 2.5
let n = 1.5
let result2 = multiply(a: m, b: n)

write(result1, " ", result2, "\n")
