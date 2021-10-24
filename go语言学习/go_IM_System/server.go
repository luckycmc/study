package main
import(
	"fmt"
	"net"
)

type Server struct{
	Ip string
	Port int
}

//创建一个server的接口
func NewServer(ip string,port int) *Server{
	
	 server := &Server{
		   Ip:ip,
		   Port:port,
	 }
	 return server
}
//处理对应的业务
func (this *Server) Handler(conn net.Conn){
     
	 fmt.Println("连接建立成功")
}
//启动服务的接口
func (this *Server) Start() {
	  
	 //socket listen 
     listener,err := net.Listen("tcp",fmt.Sprintf("%s:%d",this.Ip,this.Port))
	 fmt.Println("listen is startting....");
	 //accept
	 if err != nil{
		 fmt.Println("net.Listen err:",err)
		 return
	 }
	 //关闭监听
	 defer listener.Close()
	 //接受客户端请求
	 for{

		  //accept
		  conn,err := listener.Accept()
		  if err != nil{
			fmt.Println("net.Listen err:",err)
			continue
		 }
        // do handle
		go this.Handler(conn)
	 }
	

	 //close listen socket
}