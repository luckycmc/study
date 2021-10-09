package main

import(
	"fmt"
	"time"
)
/**
单独在主线程中操作管道, 写满了会报错, 没有数据去获取也会报错
只要在协程中操作管道过, 写满了就会阻塞, 没有就数据去获取也会阻塞
*/

// 创建一个管道
var myCh = make(chan int, 5)
func demo()  {
 var myCh = make(chan int, 5)
 //myCh<-111
 //myCh<-222
 //myCh<-333
 //myCh<-444
 //myCh<-555
 //fmt.Println("我是第六次添加之前代码")
 //myCh<-666
 //fmt.Println("我是第六次添加之后代码")

 fmt.Println("我是第六次直接获取之前代码")
 <-myCh
 fmt.Println("我是第六次直接获取之后代码")
}
func test()  {
 //myCh<-111
 //myCh<-222
 //myCh<-333
 //myCh<-444
 //myCh<-555
 //fmt.Println("我是第六次添加之前代码")
 //myCh<-666
 //fmt.Println("我是第六次添加之后代码")

 //fmt.Println("我是第六次直接获取之前代码")
 //<-myCh
 //fmt.Println("我是第六次直接获取之后代码")
}
func example()  {
 time.Sleep(time.Second * 2)
 myCh<-666
}
func main() {
 // 1.同一个go程中操作管道
 // 写满了会报错
 //myCh<-111
 //myCh<-222
 //myCh<-333
 //myCh<-444
 //myCh<-555
 //myCh<-666

 // 没有了去取也会报错
 //<-myCh

 // 2.在协程中操作管道
 // 写满了不会报错, 但是会阻塞
 //go test()

 // 没有了去取也不会报错, 也会阻塞
 //go test()

 //go demo()
 //go demo()
 
 // 3.只要在协程中操作了管道, 就会发生阻塞现象
 go example()
 fmt.Println("myCh之前代码")
 <-myCh
 fmt.Println("myCh之后代码")

 //for{
 // ;
 //}
}