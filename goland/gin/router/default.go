package router

import (
	_ "fmt"
	"gin/controller"
	"github.com/gin-gonic/gin"
)

// api模块路由
func DefaultInit(c *gin.Engine) {

	w := &controller.Default{}
	c.GET("/", w.DefaultIndex)

}
