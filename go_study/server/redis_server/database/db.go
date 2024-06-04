package database

import (
	"redis-server/datastruct/dict"
	"redis-server/interface/database"
	"redis-server/interface/resp"
	"redis-server/resp/reply"
	"strings"
)

type DB struct {
	index  int       //db  的编号
	data   dict.Dict // 对一个的数据
	addAof func(CmdLine)
}

//命令对应的执行函数
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

	argNum := len(cmdArgs)
	if arity >= 0 {
		return argNum == arity
	}
	return argNum >= -arity
}

/* ---- data Access ----- */
//GetEntity returns DataEntity bind to given key
func (db *DB) GetEntity(key string) (*database.DataEntity, bool) {

	raw, ok := db.data.Get(key)
	if !ok {
		return nil, false
	}
	entity, _ := raw.(*database.DataEntity)
	return entity, true
}

// PutEntity a DataEntity into DB
func (db *DB) PutEntity(key string, entity *database.DataEntity) int {
	return db.data.Put(key, entity)
}

// PutIfExists edit an existing DataEntity
func (db *DB) PutIfExists(key string, entity *database.DataEntity) int {
	return db.data.PutIfExists(key, entity)
}

// PutIfAbsent insert an DataEntity only if the key not exists
func (db *DB) PutIfAbsent(key string, entity *database.DataEntity) int {
	return db.data.PutIfAbsent(key, entity)
}

// Remove the given key from db 把key  移除 掉
func (db *DB) Remove(key string) {
	db.data.Remove(key)
}

// Removes the given keys from db 移除所有的key
func (db *DB) Removes(keys ...string) (deleted int) {
	deleted = 0
	for _, key := range keys {
		_, exists := db.data.Get(key)
		if exists {
			db.Remove(key)
			deleted++
		}
	}
	return deleted
}

// Flush clean database 清空数据
func (db *DB) Flush() {
	db.data.Clear()
}
