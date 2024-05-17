package main

import(
	"fmt"
	"runtime"
)

func main(){
	go func(s string){
         
		 for i:=0;i<2;i++{
			fmt.Println(s)
		 }
	}("world")
	//主协成
	for i:=0; i<2;i++{

		 //切换一下再次分配任务
		 runtime.Gosched()  //让出主协主协程 让子携程先执行
		 fmt.Println("hello")
	}
}