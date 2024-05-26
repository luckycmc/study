package tcp

import (
	"bufio"
	"context"
	"io"
	"net"
	"redis-server/lib/logger"
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
// EchoHandler 初始化
func MakeHandler() *EchoHandler{
	return &EchoHandler{}
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
	   reader := bufio.NewReader(conn)
       //获取客户端信息
	   for{
		    msg ,err :=reader.ReadString('\n')
			//错误信息的处理
			if err !=nil {
				 
				   if err == io.EOF {
					    logger.Info("connect close")
						//删除相应的客户端 在map  中删除
						handler.activeConn.Delete(client)
				   }else{
					 logger.Warn(err)
				   }
				   return
			}
			client.Waitting.Add(1)
			b := []byte(msg)
			_,_ = conn.Write(b)  //错误暂时不处理
			client.Waitting.Done()
	   }
}

//关闭处理
func(handler *EchoHandler) Close() error{
	  
	logger.Info("handler shutting down...")
	handler.closing.Set(true)
	handler.activeConn.Range(func(key, value any) bool {
		  client:= key.(*EchoClient)
		  _ =client.Conn.Close()
		  return true
	})
	return nil
}