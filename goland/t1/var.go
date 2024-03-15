package main

import(
	"fmt"
)
//函数返回值为 int 和 string 类型的数据
func foo()(int,string){  //返回值类型 int string 字符串
    
      return 10,"string_string"
}
//主函数
func main(){
	x,_:=foo()  // 自动推导类型 获取 int数据类型
	_,y:=foo()  // 获取string数据 类型

	fmt.Println("x=",x)
	fmt.Println("y=",y)
}