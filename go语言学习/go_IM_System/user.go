package main
import (
	"net"
	"strings"
)

type User struct{
	Name string
	Addr string
	C    chan string
	conn net.Conn
	server *Server  //当前用户的server
}

//创建一个用户API
func NewUser(conn net.Conn,server *Server) *User {
	
	   userAddr := conn.RemoteAddr().String()  //转换成字符串
	   
	   user := &User{

		    Name: userAddr,
			Addr: userAddr,
			C:    make(chan string),
			conn :conn,
			server : server,
	   }
       //启动监听当前User channel 消息的gorontine
	   go user.ListenMessage()

	   return user
}

//用户的上线业务
func (this *User) Online() {
	  //用户上线 将用户加入到online中
	  this.server.mapLock.Lock()
	  this.server.OnlineMap[this.Name] = this
	  this.server.mapLock.Unlock()
	  //广播当前用户消息
	  this.server.BroadCast(this,"已上线")

}
//用户的下线业务
func (this *User) Offline() {
	  //用户上线 将用户加入到online中
	  this.server.mapLock.Lock()
	  delete(this.server.OnlineMap,this.Name)
	  this.server.mapLock.Unlock()
	  //广播当前用户消息
	  this.server.BroadCast(this,"已下线")
}
//发送消息给用户
func (this *User) SendMsg(msg string)  {
	  
	  this.conn.Write([]byte(msg))
}
//用户处理消息的业务
func (this *User) DoMessage(msg string)  {
	//获取对应的命令然后解析
	if msg == "who"{
         
		  //查询当前在线用户有哪些
		  this.server.mapLock.Lock()
		  for _, user := range this.server.OnlineMap{
			  onlineMsg := "["+user.Addr+"]" +user.Name +":"+"在线...\n"
			  this.SendMsg(onlineMsg)
		  }
		  this.server.mapLock.Unlock()
	}else if len(msg) >7 && msg[:7] == "rename|"{
          
		  //消息格式 rename|张三
		  newName := strings.Split(msg, "|")[1]
		  _,ok := this.server.OnlineMap[newName]
		  if ok{
			  this.SendMsg("当前用户名被使用\n")
		  }else{
			this.server.mapLock.Lock()
			delete(this.server.OnlineMap, this.Name)
			this.server.OnlineMap[newName] = this
			this.server.mapLock.Unlock()
			this.Name = newName
			this.SendMsg("您已经更新用户名:"+this.Name+"\n")
		  }
		  
	}else if len(msg) >4 && msg[:3] == "to|" {
            
		    //消息格式 rename|张三|消息内容
			 //获取对方的用户名
		  remoteName := strings.Split(msg, "|")[1]
		  if remoteName == ""{
			  this.SendMsg("消息格式不正确,请使用 to|张三|你好呀，格式\n")
			  return 
		  }
		  //获取用户名得到 user对象
          remoteUser,ok := this.server.OnlineMap[remoteName]
		  if !ok{
			  this.SendMsg("该用户名不存在\n")
		      return
		  }
		  //获取消息内容
		  content := strings.Split(msg,"|")[2]
		  if content == ""{
			  this.SendMsg("无消息内容,请重发\n")
			  return
		  }
		  //要发送的用户
		  remoteUser.SendMsg(this.Name + "最你说:"+content+"\n")
          
	}else{
		this.server.BroadCast(this,msg) //广播用户信息
	}
	
}
//监听当前User channnel 的方法，一旦有消息，直接发送给对端客户
func (this *User) ListenMessage() {
	  
	  for{
		  msg := <-this.C

		  this.conn.Write([]byte(msg+"\n"))
	  }
}