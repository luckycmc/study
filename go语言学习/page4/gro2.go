package main
import (
	"fmt"
	"sync"
	"time"
)
//定义全局变量
var wg sync.WaitGroup  //获取同步锁
//hello grontinue
func hello1(){
   defer wg.Done()  //函数结束后执行defer
   var i int
   for{
      if i==10{
		  break
	  }
	  fmt.Println("hello Goroutine",i)
	  time.Sleep(time.Second)
	  i++
   }
   
}
func hello(i int){
	defer wg.Done()  //函数结束后执行defer
	fmt.Println("hello Goroutine",i)
	time.Sleep(time.Second)
 }
//主函数
func main(){
 
  //启动hello进程
  for i :=0;i<10;i++{
	wg.Add(1)   //开启多个同步锁
	go hello(i)
  }
  fmt.Println("main goroutine done!")
  wg.Wait()  //等待 所有的wg.Done()调用结束在往下执行
  fmt.Println("done done...")
}