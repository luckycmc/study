package main

import (
	"fmt"
	"time"
)

func worker(done chan bool) {
	fmt.Println("工作中...")
	time.Sleep(time.Second)
	fmt.Println("工作完成")
	// 发送一个值表示工作已经完成
	done <- true
}

func main() {
	// 创建一个非缓冲的布尔型通道
	done := make(chan bool, 2)
	// 启动一个工作goroutine
	go worker(done)
	//不会被阻塞
	fmt.Println("main before")
	// 等待工作goroutine的通知
	// 管道中有数据才可以往下走 管道取出数据之前的代码不会被阻塞 会继续执行
	<-done
	fmt.Println("在主goroutine中继续执行")
}
