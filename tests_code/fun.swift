func f (_ x : Int) -> Int // seznam parametru
/* deklarace funkce */ {
    if(x<10) {
      return x-1
    }
    else {
        let x = x - 1
        write("calling g with ", x, "\n")
        g(applyOn: x)
        return 10
    }
}

func g(applyOn x:Int) -> Int {
    if (x > 0) {
        write("calling f with ", x, "\n") 
        f(x) // modifikace parametru x, ale az po zavolani funkce f
        return 10
    }
    else {
        return 200
    }
}

let res = g(applyOn:10)
write("res: ", res, "\n")


