package main
import "fmt"
//Animal结构体
type Animal struct{
	name string
}
//Animal 结构体的方法
func (a *Animal) move(){
	fmt.Printf("%s会动！\n",a.name)
}
//Dog 狗
type Dog struct{
	Feet int8
	*Animal //通过嵌套匿名结构体实现继承,因此可以使用Animal中的结构体属性和方法
}
//d 结构体的方法
func (d *Dog) wang(){
	//d.names = "快快乐乐" //必须是结构体指定的字段
	fmt.Printf("%s会汪汪汪~\n", d.name) //通过继承实现属性的调用
}

func main(){
   d1 := &Dog{
	   Feet:4,
	   Animal:&Animal{ //结构体嵌套的是指针 
		   name:"乐乐",
	   },
   }
   d1.wang()
   d1.move()
}