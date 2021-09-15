package main
import "fmt"
//1.空接口类型的变量可以存储任意类型的变量。
//2.使用空接口实现可以接收任意类型的函数参数。
//3.使用空接口实现可以保存任意值的字典。

func main(){
	var x interface{}
	fmt.Printf("%T\n",x)
	s := "hellow 沙河"
	x = s
	fmt.Printf("%T\n",x)
	i := 100
	x = i
	fmt.Printf("type:%T value:%v\n", x, x)
	b := true
	x = b
	fmt.Printf("type:%T value:%v\n", x, x)
}