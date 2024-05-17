package router

import (
	_ "fmt"
	"gin/controller"
	"github.com/gin-gonic/gin"
) 

// api模块路由
func DefaultInit(c *gin.Engine) {

	w := &controller.Default{}
	//首页
	c.GET("/", w.DefaultIndex)
	//列表页
    c.GET("/list",w.DefaultList)
	//map json
	c.GET("/json",w.DefaultJson)
}
