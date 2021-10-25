package main
import (
	"net"
)

type User struct{
	Name string
	Addr string
	C    chan string
	conn net.Conn
}

//创建一个用户API
func NewUser(conn net.Conn) *User {
	
	   userAddr := conn.RemoteAddr().String()  //转换成字符串
	   
	   user := &User{

		    Name: userAddr,
			Addr: userAddr,
			C:    make(chan string),
			conn :conn,
	   }
       //启动监听当前User channel 消息的gorontine
	   go user.ListenMessage()
	   return user
}

//监听当前User channnel 的方法，一旦有消息，直接发送给对端客户
func (this *User) ListenMessage() {
	  
	  for{
		  msg := <-this.C

		  this.conn.Write([]byte(msg+"\n"))
	  }
}