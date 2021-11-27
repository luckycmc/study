package main
import (
	"fmt"
)

func main(){
	ch := make(chan int, 2)
	ch <- 3
	ch <-4
	
	//关闭管道
	close(ch)    //不关闭管道将会出现死锁
    
	//ch <-0  关闭管道不能再往里面写数据否则会出现  panic: send on closed channel

	for i := range ch{
		fmt.Println(i)
	}
}