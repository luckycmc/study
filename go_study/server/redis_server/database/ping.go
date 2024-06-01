package database

import (
	"redis-server/interface/resp"
	"redis-server/resp/reply"
)

func Ping(db *DB, args [][]byte) resp.Reply {
	return reply.MakePongReply()
}

// 初始化注册 PING
func init() {
	RegisterCommand("ping", Ping, 1)
}
