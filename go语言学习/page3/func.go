package main
import "fmt"
func sub(x, y int) (z int) {  //指定返回值直接返回返回值即可,对应的返回值
	 z = x - y
	 return 
}

func main(){
	sum := sub(5,1)
	fmt.Println(sum)
}