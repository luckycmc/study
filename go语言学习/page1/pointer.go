package main
import "fmt"
//非指针传值的应用
func m1(x int){
    x = 100
}
/**
  指针传值的应用
*/
func m2(y *int){
    *y =100
}
var y int = 666
func main(){
	a:=1  //定义变量
	b:=&a //获取变量的地址
	c:=*b //获取指针变量的值
	fmt.Println(b)
	fmt.Println(c)
	fmt.Println(y)
	fmt.Println("通过引用传值后的结果是:")
	m2(&y)  //通过引用传值出传值参数
	fmt.Println(y)
}