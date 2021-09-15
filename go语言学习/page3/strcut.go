package main
import "fmt"
//定义一个结构体
type person struct{
	name string
	city string
	age int8
}
//定义一个结构体
type person1 struct{
	name string
	city string
	age int8
}

func main(){
	var p1 person  //实例化 person的实例
	p1.name = "zpw"
	p1.city = "南京"
	p1.age  = 18 //
	fmt.Printf("%T\n",p1.name)
	fmt.Printf("p1=%v\n", p1)  //p1={沙河娜扎 北京 18}
	fmt.Println("匿名结构体######")
	//使用匿名结构体
	var user struct{Name string;Age int}
	user.Name = "zpw"
	user.Age  = 18
	fmt.Printf("%#v\n",user)
	//创建指针结构体
	var p2 = new(person)
	fmt.Printf("%T\n",p2)
	fmt.Printf("p2=%#v\n", p2)
	//结构体的默认值
	var p4  person
	fmt.Printf("p4=%#v\n", p4)
	//对键值对进行初始化
	p5 := person{
		name:"小王子",
		city:"北京",
		age :18,
	}
	fmt.Printf("p5=%#v\n", p5)
	//对结构体的指针进行初始化
	p6 := &person{
		name:"zpw",
		city:"南京",
		age:19,
	}
	fmt.Printf("p5=%#v\n", p6)
	fmt.Println("直接初始化")
	p8 := &person{
		"沙河娜扎",
		"北京",
		28,
	}
	fmt.Printf("p8=%#v\n", p8)
	//结构体内存布局
	type test struct{
		a int8
		b int8
		c int8
		d int8
	}
	fmt.Println("结构体的内存数据")
	//初始化相结构体数据  //go语言的 的内存地址是连续的
	n := test{
		1,2,3,4,
	}
	fmt.Printf("n.a %p\n", &n.a)
    fmt.Printf("n.b %p\n", &n.b)
    fmt.Printf("n.c %p\n", &n.c)
    fmt.Printf("n.d %p\n", &n.d)
}