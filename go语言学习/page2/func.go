package main
import "fmt"
/**
  函数的相关处理
*/
//函数的可变参数
//函数的可变参数是指参数的数量不是固定的可变参数可以通...来表示
func intSums(x...int) int{  //参数X本身是一个切片 []int[1 2 3 4 5 6]
	//fmt.Printf("%T",x) []int[1 2 3 4 5 6]
	//fmt.Println(x)
	sum :=0
	for _,v :=range x{
		sum += v
	}
	return sum
}
//变量的作用域
func testLocalVar2(x, y int) {
	fmt.Println(x, y) //函数的参数也是只在本函数中生效
	if x > 0 {
		z := 100   //变量z只在if语句块生效(在模块中定义的变量只能在模块中使用,if for switch)
		fmt.Println(z)
	}
	//fmt.Println(z)//此处无法使用变量z
}
//for中的变量
func testLocalVar3() {
	for i := 0; i < 10; i++ {
		fmt.Println(i) //变量i只在当前for语句块中生效
	}
	//fmt.Println(i) //此处无法使用变量i
}
func add(x, y int) int {
	return x + y
}

func sub(x, y int) int {
	return x - y
}
//主函数
func main(){
	//sum := intSums(1,2,3,4,5,6)
	//fmt.Println(sum)
	//testLocalVar3()
	c := add                         // 把add赋值给c 作为一个变量
	fmt.Printf("type of c:%T\n", c) // type of c:main.calculation
	fmt.Println(c(1, 2))            // 像调用add一样调用c

	f := add                        // 将函数add赋值给变量f1
	fmt.Printf("type of f:%T\n", f) // type of f:func(int, int) int
	fmt.Println(f(10, 20))          // 像调用add一样调用f
}
