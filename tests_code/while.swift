var c = 10
while c > 2 {
    let a = 5.5
    let b = 3.8
    let d = Int2Double(c)
    let p = (a + 3) * b / d * 1e2 - 5.0
    write(p, "\n")
    c = c - 1
}
