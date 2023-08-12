package main

import "net"

//user 类

type User struct{
    Name string
    Addr string
    C   chan string
    conn net.Conn
}

//创建一个用户API

func NewUser(conn net.Conn) *User{

   userAddr := conn.RemoteAddr().string()
   user:=&User{
   	  Name :userAddr,
   	  Addr :userAddr,
   	  C : make(chan string),
   	  conn :conn,
   }
   //启动监听当前user channel消息的
   go user.ListenMessage()

   return user
}
//监听当前 user channel的方法,一旦有消息就发送给 客户端
func (this *User) ListenMessage(){
	 for{
	 	msg:=<-this.C
	 	this.conn.Write([]byte(msg+"\r"))
	 }
}