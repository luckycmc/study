package main
import "fmt"
//定义一个结构体
type Person struct{
	name string
	age  int8
}
type Person2 struct{
	name string
	age  int8
}
//newPerson 的构造方法
func NewPserson(name string,age int8) *Person{  //返回的是person的结构体的指针
	return &Person{
		name : name,
		age  : age,
	}
}
//Dream Person 做梦方法
func (p Person) Dream(){ //方法属于特定的类型例如p属于Person类型
    fmt.Printf("%s的梦想是学好Go语言！\n", p.name)
}
//使用指针接受者
func (p *Person) SetAge(newAge int8){
   p.age = newAge
}
//主方法
func main(){
	p1 := NewPserson("zpw",25)
	fmt.Printf("%T\n",p1)
	p1.Dream()
	//var p2 Person2
	//p2.Dream() //为绑定的此对象的方法报错
	p1.SetAge(30)
	fmt.Println(p1.age)
}