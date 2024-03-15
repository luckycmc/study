package main

import "fmt"

//无缓冲的通道又称为阻塞的通道。我们来看一下下面的代码
/* 因为我们使用ch := make(chan int)创建的是无缓冲的通道，
无缓冲的通道只有在有人接收值的时候才能发送值。
就像你住的小区没有快递柜和代收点，快递员给你打电话必须要把这个物品送到你的手中，
简单来说就是无缓冲的通道必须有接收才能发送。
*/
func g1() {

}
func g2() {

}

func main() {
	//ch := make(chan int) 这个是无缓冲通道
	ch := make(chan int, 1) // 有缓冲通道 可以不用取出
	ch <- 10
	fmt.Println("发送成功")
}
