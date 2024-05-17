package main

import(
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func hello(i int){
	 //最后执行
     defer wg.Done() //go 协成执行结束就 减去1
	 fmt.Println("Hello Goroutine!",i)    
}

func main(){

	for i:=0;i<10;i++{
		wg.Add(1) // 启动一个协成就加1 其他数值不行 会出现死锁的现象 因为忙等待得不到释放
		//启动了一个携程任务
		go hello(i)
	}
	//阻塞等待为0 的时候 主携程退出
	wg.Wait() //等待登记的所有协成结束
}