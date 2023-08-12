package main

import(
	"fmt"
)

func foo()(int,string){  //返回值类型 int string 字符串
    
      return 10,"string_string"
}
func main(){
	x,_:=foo()  // 自动推导类型
	_,y:=foo()

	fmt.Println("x=",x)
	fmt.Println("y=",y)
}