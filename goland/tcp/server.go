package main
import(
	"fmt"
	"net"
)
type Server struct{
	Ip string
	Port int
}

//创建server 接口
func NewServer(ip string,port int) *Server{

	server:= &Server{
		Ip:ip,
		Port:port,
	}
    return server
}

// handler 
func (this *Server) Handler(conn net.Conn){
   fmt.Println("连接成功...")
}

//启动服务器
func (this *Server) Start(){
	//socket listen
	listener,err := net.Listen("tcp",fmt.Sprintf("%s:%d",this.Ip,this.Port))
	if err!=nil{
		fmt.Println("listen error",err)
		return //不在往下执行
	}else{
		fmt.Println("server is listenning ....\n")
	}
	//关闭连接
	defer listener.Close()
	for{
		// accept
		conn,err := listener.Accept()
		if err != nil {
			fmt.Println("listener Accept error",err)
			continue
		}
	   // do handler
	   go this.Handler(conn)
	}
	
	
}