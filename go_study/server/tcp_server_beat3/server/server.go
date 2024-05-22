package server

import (
	"fmt"
	"io"
	"net"
	"tcp_server_beat3/message"
)

type Server struct {
	IP   string
	Port int
}
//实例化server
func NewServer(ip string, port int) *Server {

	return &Server{
		IP:   ip,
		Port: port,
	}
}
// 处理单个连接的请求
func handleRequest(conn net.Conn){
	//释放链接
    defer conn.Close()

	// 读取客户端发送的数据
	buffer := make([]byte,512)
	n,err := conn.Read(buffer)
	
	 //用户下线
	if n ==0 { 
		return
	}
	//数据读取错误
	if err != nil && err != io.EOF {
		fmt.Println("Conn read err:", err)
		return
	}
	//获取用户的消息  //处理相应的业务逻辑
	msg := string(buffer[:n-1])  // n-1最后是 \n
	//解析数据 处理业务逻辑
	userMsg := message.ParseMessage(msg)
	fmt.Println(userMsg.PassWord)
    conn.Write([]byte("hello use QQ"))
}
// 启动server
func (this *Server) RunServer() {
    
	 addr := fmt.Sprintf("%s:%d",this.IP,this.Port)
	listener, err := net.Listen("tcp",addr)
	if err != nil {
		fmt.Println("Error listening:", err.Error())
        return
	}
	//关闭链接
    defer listener.Close()
	fmt.Println("Listening on "+addr+"...")
	for{
		 conn,err := listener.Accept()
		 if err!=nil {
			 fmt.Println("accept is error",err.Error())
			 continue
		 }
		 //处理业务
		 go handleRequest(conn)
	}

}