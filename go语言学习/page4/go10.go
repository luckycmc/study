package main

import(
	"fmt"
	"time"
)

//参数是整型管道
func Hu(ch chan int)  {
	//模拟睡眠两个小时
	time.Sleep(2 * time.Second)
	fmt.Println("after 2 second hu!!!")
	ch <- 100
}

//主协成
func main()  {
	
	  // 新建一个没有缓冲的信道
	  ch := make(chan int)   //应为是引用类型 所以可以相互使用

	  // 将信道传入函数，开启协程
	  go Hu(ch)
	  fmt.Println("start hu, wait...")

	  //取出对应的管道中的数据
	  var v int
	  v = <- ch
	  fmt.Println("receive:", v)  //主协成退出
}