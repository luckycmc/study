package router

import (
	"github.com/gin-gonic/gin"
	"gormlearn/api"
	"gormlearn/cms"
)

// 初始化路由
func InitRouter(r *gin.Engine) {
	//注册路由 api的路由
	api.RegisterRouter(r)
	//注册cms路由
	cms.RegisterRouter(r)
}
