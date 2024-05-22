package main

import (
	"tcp_server_beat3/server"
)

func main() {
    
	server := server.NewServer("0.0.0.0",9000)
	server.RunServer()
}