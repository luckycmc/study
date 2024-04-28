package main

import (
	"blog/router"
)


func main() {
    
	//路由的注册
	r := router.InitRouter()
	//启动服务
	r.Run()
}