package main

import "fmt"

func main() {
	var a int
	//在golang 如果定义一个变量没有赋值整型默认为0
	fmt.Println("a=",a)
	var b string
	//在golang 如果定义字符默认为空
	fmt.Println("b=",b)
	//类型自动推导类型
	var c=10.11
	fmt.Println("c=",c)
	//自动推导类型
	d:="hellow world"
	fmt.Println("d=",d)
	//同一类型的值在同一范围内可以变化变化类型
	var e int =1
	    e=22
	    //e="aa"
	fmt.Println("e=",e)
}