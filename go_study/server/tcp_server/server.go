package main

import (
	"fmt"
	"io"
	"net"
	"sync"
	"time"
)

type Server struct {
	Ip   string
	Port int
	//在线用户列表
	Onlinemap map[string]*User
	MapLock   sync.RWMutex

	//消息广播的channel
	Message chan string
}

// 创建server 接口
func NewServer(ip string, port int) *Server {

	server := &Server{
		Ip:        ip,
		Port:      port,
		Onlinemap: make(map[string]*User),
		Message:   make(chan string),
	}
	return server
}

// 监听广播的消息发送给所有的用户
func (this *Server) ListenMessage() {
	//监听消息
	for {
		msg := <-this.Message //当前协程 会阻塞者这里 直到 this.Message管道有数据

		//将所有message 发送给在线User
		this.MapLock.Lock()
		//发送给每一个在线用户
		for _, cli := range this.Onlinemap {
			cli.C <- msg // cli  是 Use的对象 把数据发送给 user监听的管道
		}
		this.MapLock.Unlock()
	}
}

// 广播消息
func (this *Server) BroadCast(user *User, msg string) {

	sendMsg := "[" + user.Addr + "]" + user.Name + ":" + msg
	// 用于通知用户上线 把数据放到 message 中
	this.Message <- sendMsg
}

// handler
func (this *Server) Handler(conn net.Conn) {
	fmt.Println("连接成功...")
	user := NewUser(conn, this)

	/****用户上线 start*****/
	// 1.在user中会吧当前 用户加入到map 中 2.出发server中的 BroadCast
	// 3.会触发 server中的 ListenMessage  4.user.C <- msg 触发 发送数据
	user.Online()
	/****用户上线 end*****/
	//监听用户是否活跃channel
	isLive := make(chan bool)
	//接受客户端的消息
	go func() {
		buf := make([]byte, 4096)
		for {
			//
			n, err := conn.Read(buf)
			//客户下线
			if n == 0 {
				user.Offline()
				return
			}
			if err != nil && err != io.EOF {
				fmt.Println("Conn read err:", err)
				return
			}
			//提取用户消息
			msg := string(buf[:n-1])
			//广播数据给用户
			user.DoMesssage(msg)
			//用户任意消息代表当前用户是活跃的
			isLive <- true
		}
	}()
	//当前handle阻塞
	for {
		// 主要用于 用户是否还活跃
		select {
		case <-isLive:
		//当前用户活跃重置制定时器
		/***不需要做任何操作 主要用于下线相应的用户 start**/
		case <-time.After(time.Second * 10):
			//已经超时当前用户被剔除
			user.SendMsg("你被强制下线")
			//销毁当前资源
			close(user.C)
			//关闭当前客户端
			conn.Close()
			//退出当前handler
			return // 或者runtime.Goexit()
			/***不需要做任何操作 主要用于下线相应的用户 end**/
		}
	}

}

// 启动服务器
func (this *Server) Start() {
	//socket listen
	listener, err := net.Listen("tcp", fmt.Sprintf("%s:%d", this.Ip, this.Port))
	if err != nil {
		fmt.Println("listen error", err)
		return //不在往下执行
	} else {
		fmt.Println("server is listenning port is", this.Port)
	}
	//关闭连接
	defer listener.Close()
	//启动监听Message 的goroutine
	go this.ListenMessage()

	for {
		// accept 获取一个新的链接
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("listener Accept error", err)
			continue
		}
		// do handler  主要用于  客户端的处理 每一个链接启动一个协程
		go this.Handler(conn)
	}

}
