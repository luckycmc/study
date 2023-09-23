package main

import (
	_"fmt"
)
func main(){
	//43 修改用户名
	server := NewServer("0.0.0.0",8888)
	server.Start()
}