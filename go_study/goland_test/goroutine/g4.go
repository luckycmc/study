package main

import(
	"fmt"
	"sync"
)

func f(n int,wg *sync.WaitGroup){
    //函数执行结束后执行  
	defer wg.Done()
	x,y := 0,1
	for i:=0;i<n;i++{
		fmt.Println(x)
		x,y = y,x+y
	}
}

func main(){
	//创建一个对象
	var wg sync.WaitGroup
	//添加两个原子计数器
	wg.Add(2)
	go f(10,&wg) //执行携程1
	go f(5,&wg)  //执行携程2
    //等待携程推出
	wg.Wait()
}