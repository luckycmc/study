package main
import(
	"fmt"
	"time"
)
/*
  并发传递机制有两种模型 1,共享内存和消息传值机制
  消息机制认为每个并发单元是自包含的、独立的个体，
  并且都有自己的变量，但在不同并发单元间这些变量不共享。
  每个并发单元的输入和输出只有一种，那就是消息
  不要通过共享内存来通信，而应该通过通信来共享内存
*/
func running(){
	var times int
	//构建一个无线循环体
	for{
		times++
		fmt.Println("tick",times)
		//延时1秒
		time.Sleep(time.Second)
	}
}
//主函数
func main(){
	//编发执行程序
	go running()
	//接受命令函输入不做任何事情
	var input string
	fmt.Scanln(&input)
}