package main
import (
	"fmt"
)
//关闭管道在发送数据则，会报panic错误
func main(){
	//创建一个整型通道
	ch := make(chan int)
	//关闭通道
	close(ch)
	//d打印通道的相应的数据信息
	fmt.Printf("ptr:%p cap:%d len:%d\n", ch, cap(ch), len(ch))
	//给通道数据
	ch <- 1  //给一个关闭的通道发送数据会报panic错误
}