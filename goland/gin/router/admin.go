package router

import(
	"github.com/gin-gonic/gin"
	"gin/controller"
)

//后台模块路由
func ApiAdminInit(c *gin.Engine){
       w := &controller.Admin{

       }
       c.GET("/admin/index",w.AdminIndex)
}