var a: Int? = nil

foo(a)

a = 5
foo(a)

a = nil
foo(a)

func foo(_ a: Int?) {
    if let a {
        write("not null\n")
    }
    else {
        write("null\n")
    }
}
