package handler

import (
	"context"
	"io"
	"strings"

	"net"
	"redis-server/database"
	databaseface "redis-server/interface/database"
	"redis-server/lib/logger"
	"redis-server/lib/sync/atomic"
	"redis-server/resp/connection"
	"redis-server/resp/parser"
	"redis-server/resp/reply"
	"sync"
)

var (
	unknownErrReplyBytes = []byte("-ERR unknow \r\n")
)

// resp  handler的定义
type RespHandler struct {
	activeConn sync.Map
	db         databaseface.Database
	closing    atomic.Boolean
}

// 实例化handler
func MakeHandler() *RespHandler {

	var db databaseface.Database

	//db = database.NewEchoDatabase()

	db = database.NewDatabase()
	return &RespHandler{
		db: db,
	}
}

// 关闭一个客户端的链接
func (h *RespHandler) closeClient(client *connection.Connection) {
	_ = client.Close()
	h.db.AfterClientClose(client)
	h.activeConn.Delete(client)
}

//实现接口的方法

func (r *RespHandler) Handle(ctx context.Context, conn net.Conn) {

	//正在关闭中
	if r.closing.Get() {
		_ = conn.Close()
	}
	client := connection.NewConn(conn)
	//存放客户端的数据信息
	r.activeConn.Store(client, struct{}{})
	//解析数据
	ch := parser.ParseStream(conn)
	//处理协议解析后的数据
	for payload := range ch {
		// 如果解析有错误
		if payload.Err != nil {

			if payload.Err == io.EOF || payload.Err == io.ErrUnexpectedEOF || strings.Contains(payload.Err.Error(), "use of closed network connection") {
				r.closeClient(client)
				logger.Info("connection closed " + client.RemoteAddr().String())
				return
			}

			//protocol error
			errReply := reply.MakeErrReply(payload.Err.Error())
			err := client.Write(errReply.ToBytes())
			if err != nil {

				r.closeClient(client)
				logger.Info("connection closed " + client.RemoteAddr().String())
				return
			}
			//继续下一轮
			continue
		}
		//exec  执行用户的请求
		// 用户发送的数据 是 空
		if payload.Data == nil {
			continue
		}

		reply, ok := payload.Data.(*reply.MultiBulkReply)
		if !ok {
			logger.Error("require multi bulk replt")
			continue
		}
		result := r.db.Exec(client, reply.Args)
		if result != nil {
			_ = client.Write(reply.ToBytes())
		} else {
			_ = client.Write(unknownErrReplyBytes)
		}
	}
}

// 关闭链接
func (r *RespHandler) Close() error {

	logger.Info("handler shutting down")
	r.closing.Set(true)
	r.activeConn.Range(func(key, value any) bool {

		client := key.(*connection.Connection)
		//关闭客户端
		_ = client.Close()
		return true
	})
	r.db.Close()
	return nil
}
