package database

import (
	"redis-server/datastruct/dict"
	"redis-server/interface/resp"
	"redis-server/resp/reply"
	"strings"
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

// 执行功能
func (db *DB) Exec(c resp.Connection, cmdLine CmdLine) resp.Reply {

	//PING SET SETNEX
	cmdName := strings.ToLower(string(cmdLine[0]))
	cmd, ok := cmdTable[cmdName]
	if !ok {
		return reply.MakeErrReply("ERR unkown command " + cmdName)
	}
	//SET k
	if !validateArity(cmd.arity, cmdLine) {
		return reply.MakeArgNumErrReply(cmdName)
	}
	fun := cmd.exector
	//set K V ->K V
	return fun(db, cmdLine[1:])
}

// 校验方法
func validateArity(arity int, cmdArgs [][]byte) bool {

	return true
}
