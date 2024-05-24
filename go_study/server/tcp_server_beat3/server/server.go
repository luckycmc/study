package server

import (
	"fmt"
	"io"
	"net"
	"tcp_server_beat3/message"
	"tcp_server_beat3/user"
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
	//fmt.Println(userMsg.PassWord)
	resultData := ""
	switch userMsg.Type {
		case "1":
			res:=user.User{}.Register(&userMsg.User)
			resultData = "注册失败"
			if res {
				resultData = "注册成功"
			}
			fmt.Println("用户注册")
		case "2":
			res:=user.User{}.Login(&userMsg.User)
			resultData = "登录失败"
			if res {
				resultData = "登录成功"
			}
			fmt.Println("用户登录")
		case "3":
			fmt.Println("添加好友")
	}
    //conn.Write([]byte("hello use QQ"))
	conn.Write([]byte(resultData))
}
// 启动server
func (this *Server) RunServer() {
    
	 addr := fmt.Sprintf("%s:%d",this.IP,this.Port) //字符串的格式转换
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
		 //处理业务 单独气动泵一个协成
		 go handleRequest(conn)
	}

}            