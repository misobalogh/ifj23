
func foo(a x: Int, b y: Double) -> Double {
    let a = Int2Double(x)
    return a + y * 2
}

func bar(a x: Int, b y: Double) -> Double {
    return y * 2.0
}

f()

func f() -> String {
    return "f"

}

foo(2, 2.5)
var a = foo(2, 2.5)
a = foo(a: 2, b: 2.5)
a = foo(2, b: 2.5)
a = foo(2, 2.5)
a = foo(a: 2, 2.5)
a = foo(a: 2, b: 2.5)
let b = foo(2, a)

bar(2, 2.5)
var c = bar(2, 2.5)
c = bar(a: 2, b: 2.5)
c = bar(2, b: 2.5)
c = bar(2, 2.5)
c = bar(a: 2, 2.5)
c = bar(a: 2, b: 2.5)
let d = bar(2, c)


