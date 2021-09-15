package main
import "fmt"
/*
  多个defer的执行顺序为“后进先出”；
  defer、return、返回值三者的执行逻辑应该是：
  return最先执行，return负责将结果写入返回值中；
  接着defer开始执行一些收尾工作；
  最后函数携带当前返回值退出。
*/

func f1() int {  //没有指定返回值变量，其值来源于return 的返回值 
	             //即使defer 返回值改变了变量 但是是与返回值无关
	x := 5
	defer func() {
		fmt.Println("ok")
		x++
		
	}()  //defer 语句在返回值后执行 
	//fmt.Println(x)
	return x
}
//第二个
func f2() (x int) {  //指定返回值y
	//var x int
	defer func() {
		fmt.Println("限制性---",x)  //defer 接收到 x = 5 然后在自加1 此时的 x =6 
		x++
		fmt.Println("限制性",x)
	}()
	return 5
}
func f3() (y int) { //不是对应的变量y 即使x变量改变但是不能和y对应
	x := 5
	defer func() {
		x++
	}()
	return x
}
func f4() (x int) {
	defer func(x int) {  //此时的x是内部变量,不是外部的引用
		x++
	}(x)
	fmt.Println(x)
	return 5
}
//defer 资源清理、文件关闭、解锁及记录时间等。
func main(){
	//fmt.Println(f1())
	//fmt.Println(f2())
	//fmt.Println(f3())
	fmt.Println(f4())
}