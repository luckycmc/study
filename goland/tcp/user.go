package main

import "net"

//user 类

type User struct{
    Name string
    Addr string
    C   chan string
    conn net.Conn
    //当前用户的server
    server *Server
}

//创建一个用户API

func NewUser(conn net.Conn,server *Server) *User{

   userAddr := conn.RemoteAddr().String()
   user:=&User{
   	  Name :userAddr,
   	  Addr :userAddr,
   	  C : make(chan string),
   	  conn :conn,
        server:server,
   }
   //启动监听当前user channel消息的
   go user.ListenMessage()

   return user
}
//用户上线业务
func(this *User) Online(){
    //用户上线将用户加入到maplock中
   this.server.MapLock.Lock()
   this.server.Onlinemap[this.Name]= this
   this.server.MapLock.Unlock()
   //广播用户上线
   this.server.BroadCast(this,"已上线")
}
//用户下线业务
func(this *User) Offline(){

    //删除对应的用户
    this.server.MapLock.Lock()
    delete(this.server.Onlinemap,this.Name)
    this.server.MapLock.Unlock()
    //广播用户上线
    this.server.BroadCast(this,"下线")
}
//用户处理消息的业务
func(this *User) DoMesssage(msg string){

   this.server.BroadCast(this,msg)

}
//监听当前 user channel的方法,一旦有消息就发送给 客户端
func (this *User) ListenMessage(){
	 for{
	 	msg:=<-this.C
	 	this.conn.Write([]byte(msg+"\r\n"))
	 }
}