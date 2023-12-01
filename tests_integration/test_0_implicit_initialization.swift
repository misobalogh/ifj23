let a: Int?
var b = a
write(a, b, "\n")

if let a {
    write("This code should never run\n")
}
else {
    write("This code should run\n")
}
