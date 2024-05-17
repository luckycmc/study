package api

import (
	"github.com/gin-gonic/gin"
)

// 注册路由
func RegisterRouter(r *gin.Engine) {

	r.GET("/save", SaveUser)
	r.GET("/get", GetUser)
	r.GET("/getall", GetAll)
}
