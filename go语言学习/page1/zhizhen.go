package main
import "fmt"
func main() {
	var i int=10
	fmt.Println("i的地址=",&i)
    //ptr 是一个指针变量
	var prt *int
	prt =&i   
	fmt.Println("ptr的值是",prt) //代表i的地址
	fmt.Println("ptr的地址",&prt)//prt变量的地址
	fmt.Println("ptr的地址",*prt)//存放地址指向的值
	*prt=9//改变指向的值    
	fmt.Println("i的地址=",i)//地址改变的值
}