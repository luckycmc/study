package main

import "fmt"

type Student struct {
	Name string
	Age  int
}

var list map[string]*Student
var info map[string]string

func main() {

	list = make(map[string]*Student)
	student := &Student{
		Name: "zpw",
		Age:  18,
	}
	student.Name = "12306"
	list["zpw"] = student
	list["zpw"].Age = 22 //引用类型的值 不能被修改
	fmt.Println(list["zpw"].Age)

	//map 中的字符串是否可以改变
	info = make(map[string]string)
	info["adress"] = "江苏"
	info["adress"] = "河南1"
	fmt.Println(info)
}
