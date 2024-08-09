package app

import (
	"goredis-main/database"
	"goredis-main/datastore"
	"goredis-main/handler"
	"goredis-main/log"
	"goredis-main/persist"
	"goredis-main/protocol"
	"goredis-main/server"

	"go.uber.org/dig"
)

var container = dig.New()

func init() {
	/**
	   其它
	**/
	// 配置加载 conf
	_ = container.Provide(SetUpConfig)
	_ = container.Provide(PersistThinker)
	// 日志打印 logger
	_ = container.Provide(log.GetDefaultLogger)

	/**
	   存储引擎
	**/
	// 数据持久化
	_ = container.Provide(persist.NewPersister)
	// 存储介质
	_ = container.Provide(datastore.NewKVStore)
	// 执行器
	_ = container.Provide(database.NewDBExecutor)
	// 触发器
	_ = container.Provide(database.NewDBTrigger)

	/**
	   逻辑处理层
	**/
	// 协议解析
	_ = container.Provide(protocol.NewParser)
	// 指令处理
	_ = container.Provide(handler.NewHandler)

	/**
	   服务端
	**/
	_ = container.Provide(server.NewServer)
}

func ConstructServer() (*server.Server, error) {
	var h server.Handler
	if err := container.Invoke(func(_h server.Handler) {
		h = _h
	}); err != nil {
		return nil, err
	}

	var l log.Logger
	if err := container.Invoke(func(_l log.Logger) {
		l = _l
	}); err != nil {
		return nil, err
	}
	return server.NewServer(h, l), nil
}

