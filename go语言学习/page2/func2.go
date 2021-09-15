package main
import "fmt"
//闭包函数等于  闭包 = 函数+引用环境
func adder () func (int) int{
	var x int //x的默认值为1
	return func (y int) int{
		  fmt.Println("-----",y,x)
		  x += y
		  return x
	}
}
/**
  被捕获到闭包中的变量让闭包本身拥有了记忆效应，
  闭包中的逻辑可以修改闭包捕获的变量，
  变量会跟随闭包生命期一直存在，闭包本身就如同变量一样拥有了记忆效应。
*/
func calc(base int) (func(int) int, func(int) int) {
	fmt.Println(base)  //base被调用一次
	add := func(i int) int {
		base += i  // 保存在自己的函数变量里面下相当于开辟了自己的一个新栈
		return base
	}

	sub := func(i int) int {
		base -= i
		return base
	}
	return add, sub
}

func main(){
	/*f := adder()    //都是引用函数的地址,在同一个闭包函数执行周期内,参数变量累加不消失
	fmt.Println(f(1))
	fmt.Println(f(2))
	f1 := adder()
	fmt.Println(f1(10))
	fmt.Println(f1(20))
	fmt.Println(&f)
	fmt.Println(&f1)*/
	f1, f2 := calc(10)
	fmt.Println(f1(1), f2(2)) //11 9
	fmt.Println(f1(3), f2(4)) //12 8
	fmt.Println(f1(5), f2(6)) //13 7
}