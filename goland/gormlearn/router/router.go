package router

import (
	"github.com/gin-gonic/gin"
	"gormlearn/api"
)

// 初始化路由
func InitRouter(r *gin.Engine) {
	//注册路由
	api.RegisterRouter(r)
}
