package main

import (
	"fmt"
	"os"
	"redis-server/config"
	"redis-server/lib/logger"
	"redis-server/resp/handler"
	"redis-server/tcp"
)

// 配置文件的名称
const configFile string = "redis.conf"

// 配置文件的一些属性
var defaultProperties = &config.ServerProperties{
	Bind: "0.0.0.0",
	Port: 6379,
}

// 配置文件是否存在
func fileExists(filename string) bool {
	info, err := os.Stat(filename)
	return err == nil && !info.IsDir()
}

func main() {
	//设置日志的配置
	logger.Setup(&logger.Settings{
		Path:       "logs",
		Name:       "godis",
		Ext:        "log",
		TimeFormat: "2006-01-02",
	})

	if fileExists(configFile) {
		config.SetupConfig(configFile)
	} else {
		config.Properties = defaultProperties
	}
	//启动tcp server 也就是 redis server
	err := tcp.ListenAndServeWithSignal(
		//配置项目
		&tcp.Config{
			Address: fmt.Sprintf("%s:%d",
				config.Properties.Bind,
				config.Properties.Port),
		},
		//处理的handler
		handler.MakeHandler())
	//handler.MakeHandler()
	//服务器启动有误
	if err != nil {
		logger.Error(err)
	}
}
