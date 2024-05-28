package database

import "redis-server/interface/resp"

type CmdLine = [][]byte

type Database interface {
	Exec(client resp.Connection,args[][]byte) resp.Reply
	Close()
	AfterClientClose(c resp.Connection)
}
type DataEntity struct{
	Data interface{}
	
}