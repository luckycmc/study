package main
import (
	"fmt"
	
)
func main(){
	
	  //声明一个通道
	  var mych chan int
	  //管道初始化
	  mych = make(chan int,3)
	  //查看管道长度的容量
	  fmt.Println("长度是", len(mych), "容量是", cap(mych))
	  //把对应的额数据写入管道
	  mych <- 666
	  fmt.Println("长度是", len(mych), "容量是", cap(mych))
	  //5.从管道中取出数据
	  num := <-mych
	  fmt.Println("从管道中取出的数据是:",num)
	  fmt.Println("长度是", len(mych), "容量是", cap(mych))
}