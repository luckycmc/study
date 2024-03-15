package main

import (
    "fmt"	
)
//使用管道实现协程同步
func deal(a,b int, c chan int){
      result := a +b
	  c <- result
}


func main(){
	//创建一个管道 数据类型为整形
	c := make(chan int,1)
    
	go deal(10,20,c)

	//从管道中接受 结果
	result := <-c
  
	fmt.Println("Sum:",result)
	fmt.Println("----before")
}