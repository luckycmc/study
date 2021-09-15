package main
import (
	"fmt"
	"runtime"
	"sync"
)
//定义常量
var(
	counter int //counter 是所有的goroutine 都要增加其变量值
	//wg 用来等待程序结束
	wg sync.WaitGroup
)
//incCounter函数增加counter变量
func incCounter(id int){
   //在函数退出时调用Done来通知main函数已经完成工作
   defer wg.Done()
   for count:=0;count<2;count++{
	   // 捕获 counter 的值
	   value := counter
	   // 当前 goroutine 从线程退出，并放回到队列
	   runtime.Gosched()
	   // 增加本地value 变量的值
	   value++
	   // 将该值保存回counter
	   counter = value
   }
}
//main函数入口
func main(){
	//计数加2,表示要等待两个goroutine
	wg.Add(2)
	//创建两个goroutine
	go incCounter(1)
	go incCounter(2)
	//等待goroutine 结束
	wg.Wait()
	fmt.Println("Final Counter",counter)
}