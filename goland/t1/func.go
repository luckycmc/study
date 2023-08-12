package main
import "fmt"

func myFunc(x,y int)(sum int,err error){
    return x+y,nil
}

func main() {
    num, _:= myFunc(1, 2)
    fmt.Println("num = ", num)
    num2, _:= myFunc2(1, 2,3)
    fmt.Println("num = ", num2)
}

func myFunc2(x,y,z int)(sum int,err error){
    if x >z{
       return x+y+z,nil
    }else{
        return z-x,nil
    }
}