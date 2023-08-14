package main

import (
    "net"
    "strings"
)
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
   //启动监听当前user channel消息的 有消息就发送给用户
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
//给当前用户发送消息
func(this *User) SendMsg(msg string){
    this.conn.Write([]byte(msg))
}
//用户处理消息的业务
func(this *User) DoMesssage(msg string){
   //查询在线用户
   if msg == "who" {
        //将所有message 发送给在线User
        this.server.MapLock.Lock()
        //发送给每一个在线用户
        for _,user:=range this.server.Onlinemap{
            onlineMsg := "["+user.Addr+"]"+user.Name+":在线...\n"
            this.SendMsg(onlineMsg)
        }
        this.server.MapLock.Unlock()

   }else if len(msg) > 7 && msg[:7] == "rename|"{
         
          //消息格式 ：rename|张三
          newName := strings.Split(msg,"|")[1]
          //查询用户名是否存在
           _,ok:=this.server.Onlinemap[newName]
           //存在
          if ok{
             this.SendMsg("当前用户名已经被使用\n")
          }else{ // 没有找到
               this.server.MapLock.Lock();
               //删除原来的名字
               delete(this.server.Onlinemap,this.Name)
               //添加新的名字
               this.server.Onlinemap[newName] = this
               this.server.MapLock.Unlock();
               //更新新的用户名
               this.Name = newName
               this.SendMsg("您已经更新用户名:"+this.Name+"\n")
          }

   }else{
      this.server.BroadCast(this,msg)
   }

}
//监听当前 user channel的方法,一旦有消息就发送给 客户端
func (this *User) ListenMessage(){
	 for{
	 	msg:=<-this.C
	 	this.conn.Write([]byte(msg+"\r\n"))
	 }
}