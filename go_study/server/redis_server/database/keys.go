package database

import (
	"redis-server/interface/resp"
	"redis-server/resp/reply"
)

//del key 相关的命令处理
// del

func execDel(db *DB, args [][]byte) resp.Reply {

	keys := make([]string, len(args))
	for i, v := range args {
		keys[i] = string(v)
	}
	deleted := db.Removes(keys...)
	return reply.MakeIntReply(int64(deleted))
}

func init() {
	//命令注册
	RegisterCommand("del", execDel, -2)
}
