package main
import (
	"fmt"
)
func main(){
	ch1 := make(chan int)
	ch2 := make(chan int)
	//开启gorontine将0-100的数据发送到ch1中 相当于生产者
	go func(){
        for i:=0; i<100; i++{
			fmt.Println("write data to channel")
			ch1 <- i
		}
		close(ch1)
	}()
	//取值  相当于消费者
	go func(){
		for{
			i,ok := <-ch1//从管道中取值
			if !ok{
				break
			}
			fmt.Println("read data to channel")
			ch2 <- i*i
		}
		close(ch2)
	}()
	//打印成ch2接受的数据
	for v := range ch2{
		fmt.Println(v)
	}
	
}