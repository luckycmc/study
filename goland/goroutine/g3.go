package main

import (
	"fmt"
	"runtime"
)

func main(){
	go func(){
		defer fmt.Println("A.defer")
        func() {
            defer fmt.Println("B.defer")
            // 结束协程 也就是当前协成退出 退到主线程
            runtime.Goexit()
            defer fmt.Println("C.defer")
            fmt.Println("B")
        }()
        fmt.Println("A")
	}()
     fmt.Println("--------")
	for{

	}
}