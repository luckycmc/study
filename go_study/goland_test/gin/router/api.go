package router

import(
	"github.com/gin-gonic/gin"
	"gin/controller"
)
// api模块路由
func ApiRouterInit(c *gin.Engine){
       
       w := &controller.Api{

       }
       c.GET("/api/index",w.ApiIndex)
       
}