package main

import(
	"fmt"
	"time"
	"math/rand"
)
/****
   无缓冲Channel和有缓冲Channel
     有缓冲管道具备异步的能力(写几个读一个或读几个)
    无缓冲管道具备同步的能力(写一个读一个)
*/
var myCh   = make(chan int,5)      //也就是缓冲区 相当于c语言中的buf
var exitCh = make(chan bool, 1)
/*****
   生产者
*/
func producer(){
    
	 rand.Seed(time.Now().UnixNano())
	 for i := 0; i < 100; i++{
           
		   num := rand.Intn(120)
		   fmt.Println("生参则生参了:",num)
		   //数据写入管道
		   myCh <- num

	 }

	 //生参完毕关闭管道
	 close(myCh)
	 fmt.Println("数据生参完毕")
}

//消费者
func consumer(){
      
	 //从管道中一直取数据知道数据取出完毕为止
	 for{

		if num, ok := <-myCh; !ok{
			  break  //终止取数据
		 }else{
			 fmt.Println("-----消费者消费了",num)
		 }
		
	 }
	 fmt.Println("消费者停止消费")
	 exitCh <- true  //数据写入管道
}
//入口函数
func main() {
     
	  go producer();
	  go consumer();

	  fmt.Println("exitCh before")
	   <-exitCh       //时候终止主线程  没有数据会一直阻塞在这里
	  fmt.Println("exitCh after")
}