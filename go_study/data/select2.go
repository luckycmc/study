package main

import (
	"fmt"
	"time"
)

func main() {
	//创建管道
	outpu1 := make(chan string)
	//子进程写数据
	go write(outpu1)

	for data := range outpu1 {
		fmt.Println("result is :",data)
		time.Sleep(time.Second * 2)
	}
}

func write(str chan string) {  

	 for{

		 select{
		 case str <- "hello":
			 fmt.Println("write hello")
		 default:
              fmt.Println("channel is full")
		 }
		 time.Sleep(time.Microsecond * 500)
	 }

}