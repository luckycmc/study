package main

import(
   "fmt"
   "time"
)
func Hu()  {
	//模拟睡眠两个小时
	time.Sleep(2 * time.Second)
	fmt.Println("after 2 second hu!!!")
}
/**
  因为 main 函数本身作为程序的主协程，如果 main 函数结束的话，
  其他协程也会死掉，必须使用死循环来避免主协程终止。
**/
func main()  {
	 
	 //阻塞运行  只有第一个函数执行完毕才执行第二个结构体
	 //Hu(); 阻塞执行

	 go Hu();   //相当于开启两个线程 分别执行

	 fmt.Println("start hu, wait...")
	 for{
		 time.Sleep(time.Second)
	 }
}