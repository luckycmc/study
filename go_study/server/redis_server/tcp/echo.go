package tcp

import (
	"context"
	"net"
	"redis-server/lib/sync/atomic"
	"redis-server/lib/sync/wait"
	"sync"
	"time"
)

//客户端的信息
type EchoClient struct{
	Conn net.Conn
	Waitting wait.Wait
}

func(e *EchoClient) Close() error{
      
	  e.Waitting.WaitWithTimeout(10 * time.Second)
	 _= e.Conn.Close()
	 return nil
}

type EchoHandler struct {
	activeConn sync.Map
	closing atomic.Boolean
}
//客户端的业务逻辑处理
func(handler *EchoHandler) Handle(ctx context.Context,conn net.Conn){
        
	   //链接正在关闭的直接关闭
	   if handler.closing.Get() {
		    _ =conn.Close()
	   }
	   client := &EchoClient{
		  Conn: conn,
	   }
        //存放数据 用空结构体
	   handler.activeConn.Store(client,struct{}{})

}


func(handler *EchoHandler) Close() error{
	  
	return nil
}