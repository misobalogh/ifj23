func f (_ x : Int)// seznam parametru
/* deklarace funkce */ {
  if(x<10){return x-1}else{let x = x - 1
    write("calling g with ", x, "\n")
    let res = g(applyOn: x)
    return 1
  }
}

func g(applyOn x:Int) -> Int {
  if (x > 0) {
    write("calling f with ", x, "\n") 
    let x = f(x) // modifikace parametru x, ale az po zavolani funkce f
  } else {
  
  return 200
  
  }
}

let res = g(applyOn:10)
write("res: ", res, "\n")


