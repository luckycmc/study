package main

import(
	"fmt"
	"time"
	"runtime"
)

func main(){
	var a [10]int
	for i := 0; i < 10; i++ {
		  go func(i int){
              for{ //当等于PHP 的while
              	a[i]++
              	//会主动让出 cpu 让其他协成执行
              	runtime.Gosched() //其他协层也能得到cpu的时间片运行
              }
		  }(i)
	}
	time.Sleep(time.Millisecond)
	fmt.Println(a)
}