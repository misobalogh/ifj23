func bar(with param : String)  {
    foo(param)
}
func foo(_ par : String) {
    bar(with: par)
}
bar(with: "ahoj")
