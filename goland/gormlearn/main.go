package main

import (
	_ "fmt"
	"github.com/gin-gonic/gin"
	"gormlearn/router"
	"log"
)

func main() {
	//初始化gin框架
	r := gin.Default()
	//加载对应的视图魔板
	r.LoadHTMLGlob("templates/**/*")
	//注册路由
	router.InitRouter(r)
	//启动服务
	err := r.Run()
	if err != nil {
		log.Fatalln(err)
	}
}
