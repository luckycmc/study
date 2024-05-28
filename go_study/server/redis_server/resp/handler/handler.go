package handler

import (
	"context"
	"net"
	databaseface "redis-server/interface/database"
	"redis-server/lib/logger"
	"redis-server/lib/sync/atomic"
	"redis-server/resp/connection"
	"sync"
)

// resp  handler的定义
type RespHandler struct {
	activeConn sync.Map
	db  databaseface.Database
	closing atomic.Boolean
}

//实现接口的方法

func(r RespHandler) handler(ctx context.Context,conn net.Conn){

}

//关闭链接
func(r *RespHandler) Close() error{
     
	logger.Info("handler shutting down")
	r.closing.Set(true)
	r.activeConn.Range(func(key, value any) bool {
        
		client := key.(*connection.Connection)
		//关闭客户端 
		_=client.Close()
		return true
	})
	r.db.Close()
	return nil
}