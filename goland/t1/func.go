package main
import "fmt"

func myFunc(x,y int)(sum int,err error){
    return x+y,nil
}

func main() {
    num, _:= myFunc(1, 2)
    fmt.Println("num = ", num)
}