package main
import (
	"fmt"
	"net"
)

//处理与客户端的连接
func process(conn net.Conn)  {
	  
	defer conn.Close() //延时关闭客户端
	
	//在这里掉一个总控
	processor := &Processor{
		Conn:conn,
	}
	err := processor.Process2()
	if err !=nil {
		fmt.Println("客户端和服务器端通讯的协成出问题了 = err",err)
		return
	}
}

//主函数
func main()  {
	
	  //提示新消息
	  fmt.Println("服务器在8090端口监听.....")
      //直接进入监听状态
	  listen,err := net.Listen("tcp","0.0.0.0:8889")  //和C语言不同的是C需要 socket bind listen
	  defer listen.Close() //延时关闭客户端
	  if err != nil {
		  fmt.Println("listen failed: ",err)
		  return //程序终止
	  }
	  //一旦监听成功 等待客户端连接服务器
	  for{

		   fmt.Println("等待客户端连接服务器")
		   conn,err := listen.Accept()
		   if err != nil {
			   fmt.Println("listen.Accept err",err)
			   continue  //当前循环跳出
		   }
           
		   //一旦连接成功,则启动一个协成和客户端保持通讯
		    go process(conn)
	  }
}