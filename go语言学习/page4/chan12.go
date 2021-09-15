package main
import "fmt"
//缓冲通道在关闭后依然可以访问内部的数据。
func main(){
	ch := make(chan int,2)
	//通道放入两个数据
	ch <- 0
	ch <- 1
	//ch <- 2  //存放数据不能操出管道的容量
	//关闭通道
	close(ch)
	//遍历缓缓冲管道的数据
	for i:=0; i<cap(ch)+1; i++{
		//从滚到中取数据
		v,ok := <-ch

		//打印去除数据的状态
	    fmt.Println(v,ok)
	} 
	//取数据越界时，会返回false 默认值为0
}