package main
import "fmt"

// 接受管道的数据
func rec(c chan int){
	ret := <-c  //等待管道的值发送过来,
	fmt.Println("数据接受成功",ret)
}
func main(){
   //定义一个整型通道
   ch := make(chan int) //无缓存通道
   go rec(ch)
   ch <- 10 //赋值10给ch通道
   //形成阻塞,直到管道里的值取出才释放资源，代码程序继续往下走
   fmt.Println("发送成功!")
}