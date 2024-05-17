package router

import (
	"github.com/gin-gonic/gin"
)
//路由初始化
func InitRouter(r *gin.Engine){
	InitBookRouter(r)
}