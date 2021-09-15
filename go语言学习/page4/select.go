package main
import (
	"fmt"
	"time"
)
//主函数
func main(){
   
	timeout := make(chan bool,1) //有缓存通道
	ch      := make(chan int)    //无缓存通道
	//匿名函数
	go func(){
		time.Sleep(1)  //等待一秒
		fmt.Println("a is starting...")
		timeout <- true
	}()
	//吧select和channel利用起来
	select{
	   case <-ch:   //单独一个会形成死锁dealLock 没有复制一直处于等待的状态
	   case a :=<-timeout:  //select 只要有一个成立则程序就往下执行
		fmt.Println(a)
	} 
	fmt.Println("channle end...")
}