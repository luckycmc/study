package main
import "fmt"
//没用名称的函数称为闭包函数
// 闭包函数在结尾调用自己或者用变量调用自己
func main() {
	// 将匿名函数保存到变量
	add := func(x, y int) {
		fmt.Println(x + y)
	}
	add(10, 20) // 通过变量调用匿名函数 //闭包变量是引用类型

	//自执行函数：匿名函数定义完加()直接执行
	func(x, y int) {
		fmt.Println(x + y)
	}(10, 20)
	fmt.Println(&add)
	fmt.Println(add)
}