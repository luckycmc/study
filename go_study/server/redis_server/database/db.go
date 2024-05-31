package database

import (
	"redis-server/datastruct/dict"
	"redis-server/interface/resp"
)

type DB struct {
	index int       //db  的编号
	data  dict.Dict // 对一个的数据
}

// 执行函数
type ExecFunc func(db *DB, args [][]byte) resp.Reply

type CmdLine = [][]byte

func makeDB() *DB {

	db := &DB{
		data: dict.MakeSyncDict(),
	}
	return db
}
