package main

import (
	"net"
	"strings"
)

// user 结构体
type User struct {
	Name string
	Addr string
	C    chan string // 主要用于发送给 用户的消息
	conn net.Conn
	//当前用户的server
	server *Server
}

//创建一个用户API

func NewUser(conn net.Conn, server *Server) *User {

	userAddr := conn.RemoteAddr().String()
	user := &User{
		Name:   userAddr,
		Addr:   userAddr,
		C:      make(chan string), // 初始化
		conn:   conn,
		server: server,
	}
	//启动监听当前user channel消息的 有消息就发送给用户
	go user.ListenMessage()

	return user
}

// 用户上线业务
func (this *User) Online() {
	//用户上线将用户加入到maplock中
	this.server.MapLock.Lock()
	this.server.Onlinemap[this.Name] = this
	this.server.MapLock.Unlock()
	//广播用户上线
	this.server.BroadCast(this, "已上线")
}

// 用户下线业务
func (this *User) Offline() {

	//删除对应的用户
	this.server.MapLock.Lock()
	delete(this.server.Onlinemap, this.Name)
	this.server.MapLock.Unlock()
	//广播用户下线
	this.server.BroadCast(this, "下线")
}

// 给当前用户发送消息
func (this *User) SendMsg(msg string) {
	this.conn.Write([]byte(msg))
}

// 用户处理消息的业务 这一块业务也可以继续拆分
func (this *User) DoMesssage(msg string) {
	//查询在线用户
	if msg == "who" {
		//将所有message 发送给在线User
		this.server.MapLock.Lock()
		//发送给每一个在线用户
		for _, user := range this.server.Onlinemap {
			onlineMsg := "[" + user.Addr + "]" + user.Name + ":在线...\n"
			this.SendMsg(onlineMsg)
		}
		this.server.MapLock.Unlock()

	} else if len(msg) > 7 && msg[:7] == "rename|" {

		//消息格式 ：rename|张三
		newName := strings.Split(msg, "|")[1]
		//查询用户名是否存在
		_, ok := this.server.Onlinemap[newName]
		//存在
		if ok {
			this.SendMsg("当前用户名已经被使用\n")
		} else { // 没有找到
			this.server.MapLock.Lock()
			//删除原来的名字
			delete(this.server.Onlinemap, this.Name)
			//添加新的名字
			this.server.Onlinemap[newName] = this
			this.server.MapLock.Unlock()
			//更新新的用户名
			this.Name = newName
			this.SendMsg("您已经更新用户名:" + this.Name + "\n")
		}
		//消息私聊功能
	} else if len(msg) > 4 && msg[:3] == "to|" {
		//消息格式: to|张三|消息内容
		//1.获取用户名查找用户名
		remoteName := strings.Split(msg, "|")[0]
		//1.用户名为空
		if remoteName == "" {
			this.SendMsg("消息格式不正确,请使用:to|zhangsan|你好a|")
			return
		}
		//2.更具用户名 得到当前对象
		remoteUser, ok := this.server.Onlinemap[remoteName]
		if !ok {
			this.SendMsg("该用户名不存在\n")
			return
		}
		//3.获取消息信息
		content := strings.Split(msg, "|")[2]
		if content == "" {
			this.SendMsg("消息内容不能为空\n")
			return
		}
		//把消息发送给指定用户
		remoteUser.SendMsg(this.Name + "对你说:" + content)

	} else {
		this.server.BroadCast(this, msg)
	}

}

// 主要用于 通知 某个用户上线 start
// 监听当前 user channel的方法,一旦有消息就发送给 客户端 
func (this *User) ListenMessage() {
	for {
		msg := <-this.C
		this.conn.Write([]byte(msg + "\r\n"))
	}
}
// 主要用于 通知 某个用户上线 end