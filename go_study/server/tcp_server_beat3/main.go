package main

import (
	"tcp_server_beat3/model"
	"tcp_server_beat3/server"
)

func init(){
	//初始化 mysql  链接
	model.InItDb()
	//初始化 redis  链接
	model.InitRedis()
}
func main() {
    
	server := server.NewServer("0.0.0.0",9000)
	server.RunServer()
}