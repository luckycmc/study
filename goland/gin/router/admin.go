package router

import (
       "gin/controller"
       "github.com/gin-gonic/gin"
)

// 后台模块路由
func ApiAdminInit(c *gin.Engine) {
       w := &controller.Admin{}
       //index后台首页
       c.GET("/admin/index", w.AdminIndex)
       //list 列表页
       c.GET("/admin/list", w.AdminList)
}
