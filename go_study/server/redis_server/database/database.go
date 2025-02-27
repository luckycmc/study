package database

import (
	"fmt"
	"redis-server/aof"
	"redis-server/config"
	"redis-server/interface/resp"
	"redis-server/lib/logger"
	"redis-server/resp/reply"
	"runtime/debug"
	"strconv"
	"strings"
)

// Database is a set of multiple database set
type Database struct {
	dbSet []*DB
	// handle aof persistence
	aofHandler *aof.AofHandler
}

// NewDatabase creates a redis database,
func NewDatabase() *Database {
	mdb := &Database{}
	if config.Properties.Databases == 0 {
		config.Properties.Databases = 16
	}
	mdb.dbSet = make([]*DB, config.Properties.Databases)
	for i := range mdb.dbSet {
		singleDB := makeDB()
		singleDB.index = i
		mdb.dbSet[i] = singleDB
	}
	//aof 是否启动 启动的话就需要
	if config.Properties.AppendOnly {
		
		aofHandler, err := aof.NewAOFHandler(mdb)
		if err != nil {
			panic(err)
		}
		mdb.aofHandler = aofHandler
		for _, db := range mdb.dbSet {
			// avoid closure
			singleDB := db
			singleDB.addAof = func(line CmdLine) {
				mdb.aofHandler.AddAof(singleDB.index, line)
			}
		}
	}
	return mdb
}

// Exec executes command  执行redis  命令
// parameter `cmdLine` contains command and its arguments, for example: "set key value"
func (mdb *Database) Exec(c resp.Connection, cmdLine [][]byte) (result resp.Reply) {

	defer func() {
		if err := recover(); err != nil {
			logger.Warn(fmt.Sprintf("error occurs: %v\n%s", err, string(debug.Stack())))
			result = &reply.UnknownErrReply{}
		}
	}()
	// 命令的名称
	cmdName := strings.ToLower(string(cmdLine[0]))
	// select 的操作
	if cmdName == "select" {
		if len(cmdLine) != 2 {
			return reply.MakeArgNumErrReply("select")
		}
		return execSelect(c, mdb, cmdLine[1:])
	}
	// normal commands
	dbIndex := c.GetDBIndex()
	if dbIndex >= len(mdb.dbSet) {
		return reply.MakeErrReply("ERR DB index is out of range")
	}
	// 对应库的执行
	selectedDB := mdb.dbSet[dbIndex]
	//执行相应的redis 指令
	return selectedDB.Exec(c, cmdLine)
}

// Close graceful shutdown database
func (mdb *Database) Close() {

}

func (mdb *Database) AfterClientClose(c resp.Connection) {

}

// select 选择数据库
func execSelect(c resp.Connection, mdb *Database, args [][]byte) resp.Reply {
	dbIndex, err := strconv.Atoi(string(args[0]))
	if err != nil {
		return reply.MakeErrReply("ERR invalid DB index")
	}
	if dbIndex >= len(mdb.dbSet) {
		return reply.MakeErrReply("ERR DB index is out of range")
	}
	c.SelectDB(dbIndex)
	return reply.MakeOkReply()
}
