package main
import "fmt"
func main(){
	// 声明切片类型
	/*var a []string              //声明一个字符串切片
	var b = []int{}             //声明一个整型切片并初始化
	var c = []bool{false, true} //声明一个布尔切片并初始化
	//var d = []bool{false, true} //声明一个布尔切片并初始化
	fmt.Println(a)              //[]
	fmt.Println(b)              //[]
	fmt.Println(c)              //[false true]
	fmt.Println(a == nil)       //true
	fmt.Println(b == nil)       //false
	fmt.Println(c == nil)       //false
	//fmt.Println(c == d)   //切片是引用类型，不支持直接比较，只能和nil比较*/
	//切片拥有自己的长度和容量，len()函数求长度，cap()函数是用来求容量
	a:=[5]int{55,56,57,58,59}//定义一个数组
	b:=a[1:4]  //基于的数组创建切片(左闭右开)也就是包含左边不包含右边
	fmt.Println(b)
	fmt.Printf("type of b:%T\n",b) //type of b:[]int 表示切片类型
	a1:= [...]string{"北京", "上海", "广州", "深圳", "成都", "重庆"}//定义一个字符串数组
	fmt.Printf("type of a1:%T\n",a1)
	fmt.Printf("a:%v type:%T len:%d  cap:%d\n", a1, a1, len(a1), cap(a1))
	b1 := a1[0:3] //切片的宽短是从起始位置到结束
	fmt.Printf("b:%v type:%T len:%d  cap:%d\n", b1, b1, len(b1), cap(b1))
	c := a1[1:5]
	fmt.Printf("c:%v type:%T len:%d  cap:%d\n", c, c, len(c), cap(c))
	aaa:=&a
	fmt.Println(aaa)
	fmt.Println("******************************************切片是值引用")
	s1:=make([]int,3)//定义一个切片并初始化
	s2:=s1          //切片赋值是引用传值,公用一个内存地址
	s2[0] = 100
	fmt.Println(s1)
	fmt.Println(s2)
	s3:=make([]int,3,3)
	copy(s3,s1)  //cop()函数是整体copy一份到新的内存中
	fmt.Printf("s1 的地址%p\n",s1) //s1 的地址0xc042052120
	fmt.Printf("s2 的地址%p\n",s2) //s2 的地址0xc042052120  s1和s2内存地址一样
	fmt.Printf("s3 的地址%p\n",s3) //s3 的地址0xc042052180 //新的内存地址
	fmt.Println("******************************************切片的遍历")
	for k,v:=range s1{
		fmt.Println(k,v)
	}
	for i:=0;i<len(s1);i++{
		fmt.Println(s1[i])
	}
	s1=append(s1,666,777,888,9999) //追加相应的数据
	fmt.Println(s1)
	fmt.Printf("s1 的地址%p\n",s1)
	
}