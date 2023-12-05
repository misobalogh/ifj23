
func foo(_ n: Int) {
    if n < 10 {
        write("foo ", n, "\n")
        let a = n + 1
        bar(a)
    }
    else {}
}

func bar(_ n: Int) {
    if n < 10 {
        write("bar ", n, "\n")
        let a = n + 2
        foo(a)
    }else {}
}

foo(1)
