package main
import (
	"errors"
	"fmt"
	"time"
)
//模拟RPC客户端请求和发送数据
func RPCClient(ch chan string,req string)(string,error){
	  //向服务器发送数据
	  ch <- req  //通过管道
	  //等待服务器放回数据
	  select{
		   case ack := <-ch:  //接收到服务器返回的数据
			   return ack,nil
		   case  <- time.After(time.Second): //超时一秒执行相应的操作
		        return "",errors.New("Time out")
	  }
}
//模拟RPC服务器接收客户端请求和相应
func RPCServer(ch chan string){

	 for{
		 //接收客户端请求的数据
		 data := <-ch
		 // 打印接收到的数据
		 fmt.Println("server received:", data)
		 // 通过睡眠函数让程序执行阻塞2秒的任务
        //time.Sleep(time.Second * 2)
		 //向客户端反馈已收到
		 ch <- "roger"  //通过管道
	 }
	 
	   
}
func main(){
	//创建一个无缓存字符串通道
	ch := make(chan string)
	//并发执行服务器的逻辑
	go RPCServer(ch)
	//客户端请求数据和接受数据
	recv,err := RPCClient(ch,"hi")
	if err != nil{
		//发生错误打印
		fmt.Println("err")
	}else{
		//正常接收数据
		fmt.Println("client recevied",recv)
	}
}