package main
import (
	"fmt"
	"time"
)
func main(){
	ch := make(chan int,1)
	for i:=0; i<10 ;i++{
		select{
		case x := <-ch:  //从通道中取值
			fmt.Println(x)
		case ch <- i:  //从管道中发送数据,把数据放入管道
		    fmt.Println("---",i)  
		}
		time.Sleep(time.Second)
		
	} 
	//fmt.Println(ch)
}