package main
import (
	"fmt"
	"time"
)
func main(){
	//声明一个退出通用管道
	exit := make(chan int)
	//开始打印
	fmt.Println("start...")
	//过一秒后调用匿名函数
	time.AfterFunc(time.Second,func(){
		//一秒后打印结果
		fmt.Println("one second after")
		//通知main的grotinue已经结束了
		exit <- 0
	})
	//等待结束
	<-exit
}