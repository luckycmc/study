package main

import(
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func hello(i int){
     defer wg.Done() //go 协成执行结束就 减去1
	 fmt.Println("Hello Goroutine!",i)    
}

func main(){

	for i:=0;i<10;i++{
		wg.Add(1) // 启动一个协成就加1
		go hello(i)
	}
	wg.Wait() //等待登记的所有协成结束
}