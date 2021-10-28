package main
import(
	"fmt"
	"net"
	"sync"
	"io"
	"time"
)
//server struct
type Server struct{
	Ip string
	Port int
	//在线用户列表
	OnlineMap map[string]*User  //在线用户有多少
	mapLock sync.RWMutex
	//消息广播的channel
	Message chan string     //广播用户的消息
}

//创建一个server的接口
func NewServer(ip string,port int) *Server{
	
	 server := &Server{
		   Ip:ip,
		   Port:port,
		   OnlineMap : make(map[string]*User),  //初始化 map
		   Message : make(chan string),         //初始化 channel
	 }
	 return server
}
//监听message 广播所有的 协成 一旦有消息就发送给全部的在线User
func (this *Server) ListenMessage()  {
	  
	   for{
		   msg := <- this.Message

		   //把消息发送给所有的在线用户
		   this.mapLock.Lock()
		   for _,cli := range this.OnlineMap{
			   cli.C <- msg
		   }
		   this.mapLock.Unlock()
	   }
}
//广播消息
func (this *Server) BroadCast(user *User,msg string)  {
	  
	  sendMsg := "[" + user.Addr+"]" +user.Name+":"+msg
	  this.Message <- sendMsg
}
 
//处理对应的业务
func (this *Server) Handler(conn net.Conn){
     
	 //defer conn.Close(); //close
	 //fmt.Println("连接建立成功")
	 user := NewUser(conn,this)
	 //用户上线 将用户加入到online中
     user.Online()
	 //监听用户是否是活跃的管道channel
	 isLive := make(chan bool)
	 //接受客户端消息
	 go func ()  {
		   
		    buf := make([]byte, 4096)
			//读取用户发送的信息
			for{
                
				  n,err := conn.Read(buf)
				  //客户端关闭
				  if n==0 {
					    //当前用户下线
					    user.Offline()
						return
				  }
				  //数据读取错误
				  if err != nil && err != io.EOF {
					  
					  fmt.Println("Conn Read err:",err)
					  return
				  }
				  //提取用户消息（去除\n）
				  msg := string(buf[:n-1])

				  //用户针对message 
				  user.DoMessage(msg)
				  //用户的任意消息代表当前用户是一个活跃用户
				  isLive <- true
			}
	 }()

	 //当前handle 阻塞
	 for{
         
		select{
		case <- isLive:
			//当前用户是活跃的,应该重置定时器
			//不做任何事情 为了激活select 更新下面的定时器
		case <- time.After(time.Second *60):
			//已经超时
			//将当前的User强制关闭
			user.SendMsg("你被踢出了")

			//销毁用的资源
			close(user.C)
			//关闭连接
			conn.Close()
			//退出当前协成
			return
		}
	 }
	
}
//启动服务的接口
func (this *Server) Start() {
	  
	 //socket listen 
     listener,err := net.Listen("tcp",fmt.Sprintf("%s:%d",this.Ip,this.Port))
	 fmt.Println("listen is startting....",this.Ip,this.Port);
	 //accept
	 if err != nil{
		 fmt.Println("net.Listen err:",err)
		 return
	 }
	 //关闭监听
	 defer listener.Close()
	 //启动监听Message 的goroutine  用户的上线通知消息
	 go this.ListenMessage()

	 //接受客户端请求
	 for{

		  //accept
		  conn,err := listener.Accept()
		  if err != nil{
			fmt.Println("net.Listen err:",err)
			continue
		 }
        // do handle  处理用户的业务逻辑
		go this.Handler(conn)
	 }
	
	 //close listen socket
}