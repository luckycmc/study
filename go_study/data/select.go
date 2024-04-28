package main

import (
	"fmt"
	"time"
)

func t1(ch chan string) {
	time.Sleep(time.Second * 5)
	ch <- "hello"
}

func t2(ch chan string){
	time.Sleep(time.Second *3)
	ch <- "world"
}
func main() {
     
	  str1 := make(chan string)
	  str2 := make(chan string)

	  go t1(str1)
	  go t2(str2)
      //选择其中的一个 通道执行  只要有数据 就执行
		select{
           
		case s1 :=<-str1:
			fmt.Println(s1)
		case s2 :=<-str2:
			fmt.Println(s2)
        }
	
	  
}