package main

import (
	"fmt"
	"time"
)

func t1(t1 chan string) {
	time.Sleep(1 * time.Second)
	fmt.Println("I am is t1 go")
	t1 <- "hello"
}
func t2(t2 chan string) {
	fmt.Println("I am t2 go")
	c := <-t2 //如果通道没有数据这个地方将会阻塞 等待数据的到来
	fmt.Println(c)
}
func main() {

	ch := make(chan string)
	go t1(ch)
	go t2(ch)
	//不会阻塞 主程序
	fmt.Println("main is start")
	time.Sleep(3 * time.Second)
}
