package main
import "fmt"
// 提供一个值, 每次调用函数会指定对值进行累加
func Accumulate (value int ) func() int{ //返回值是闭包函数
	return func() int{   //返回一个闭包函数
		 //数据累计加一
		 value++
		 //返回对应的数据
		 return value
	}
}
func main(){
   // 创建一个累加器, 初始值为1
   accumulator := Accumulate(0) //在accumulator 
   // 累加1并打印
   fmt.Println(accumulator())
   fmt.Println(accumulator())
   // 打印累加器的函数地址
   fmt.Println(&accumulator)
   // 创建一个累加器, 初始值为1
   accumulator2 := Accumulate(10)
   // 累加1并打印
   fmt.Println(accumulator2())
   // 打印累加器的函数地址
   fmt.Println(&accumulator2)
}