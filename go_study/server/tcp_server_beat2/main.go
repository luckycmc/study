package main

import (
	_ "fmt"
)
func main(){
	//获取服务对象实例
	server := NewServer("0.0.0.0",8888)
	//启动tcp服务器
	server.Start()
}