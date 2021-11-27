package main

import(
	"fmt"
	"time"
)

//接受数据
func Recive(ch chan int)  {
	 
     //先等待几秒再接受数据
	 time.Sleep(2 * time.Second)
	 for{
		 select{
			 //接受管道中的数据
		 case v,ok := <-ch:
			  
			   //如果管道被关闭则退出
			   if !ok {
				   fmt.Println("chan close ,receive:",v)
				   return
			   }
			   fmt.Println("receive:",v)
		 }
	 }
}

//发送数据
func Send(ch chan int)  {
	 for i := 0; i < 13; i++ {
		  
		  ch <- i //数据写入管道
		  fmt.Println("send:", i)
	 }
    //打印完毕关闭管道
	close(ch)
}

//主函数

func main(){

	 ch := make(chan int, 10)
	 go Send(ch)
	 go Recive(ch)
     // 必须死循环，不然主协程退出了，程序就结束了
	 for{
        
		  time.Sleep(time.Second * 1)
	 }
}