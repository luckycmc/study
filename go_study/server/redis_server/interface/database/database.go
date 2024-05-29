package database

import "redis-server/interface/resp"

type CmdLine = [][]byte

type Database interface {
	Exec(client resp.Connection,args[][]byte) resp.Reply
	Close()
	AfterClientClose(c resp.Connection)
}
//数据类型
type DataEntity struct{
	Data interface{}
}