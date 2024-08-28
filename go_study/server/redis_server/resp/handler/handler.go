package handler

import (
	"context"
	"io"
	"strings"

	"net"
	database2 "redis-server/database"
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
	// 输出处理
	//db = database.NewEchoDatabase()
	//解析处理 的相应接口
	db = database2.NewDatabase()
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
	//获取到客户端的属性
	client := connection.NewConn(conn)
	//存放客户端的数据信息
	r.activeConn.Store(client, struct{}{})
	//解析数据 redis-client 发送过来的数据
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
			logger.Error("empty payload")
			continue
		}

		//fmt.Println(payload)
		// 解析 服务器 什么意思？ golang 类型断言 

		// 解析 服务器 什么意思？
		// 类型断言 判断是都是 二位的 byte 类型  接口的反射
		// https://www.bilibili.com/video/av330409643/?vd_source=d5bdbf7b72f24c50670432a46f350cb0
        // 是否是二位 数组 byte 类型
		reply, ok := payload.Data.(*reply.MultiBulkReply)
		if !ok {
			logger.Error("require multi bulk replt")
			continue
		}
		//执行解析的命令 调用对应的函数处理
		result := r.db.Exec(client, reply.Args)
		if result != nil {
			_ = client.Write(result.ToBytes())
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
