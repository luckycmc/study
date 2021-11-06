package main
import (
	"fmt"
	"net"
)
//处理与客户端的连接
func process(conn net.Conn)  {
	 
	  //读取客户端发送的数据
}
func main()  {
	
	  //提示新消息
	  fmt.Println("服务器在8889端口监听.....")
      listen,err := net.Listen("tcp","0.0.0.0:8889")
	  if listen != nil {
		   fmt.Println("net.Listen err= ",err)
		   return 
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