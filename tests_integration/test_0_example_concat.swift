func concat(_ p : String, with y : String) -> String {
    let x = p + y
    return x + " " + y
}
let a = "ahoj "
var ct : String
ct = concat(a, with: "svete")
write(ct, a)
