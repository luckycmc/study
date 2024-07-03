package routes

import (
	"lottery/web/controller"

	"github.com/gin-gonic/gin"
)

func InitRouter() *gin.Engine{

	r := gin.Default()
	//加载静态资源
	r.Static("public","web/public")
	//加载模板文件
	r.LoadHTMLGlob("web/templates/**/*")
	//处理路由请求
	dealRouter(r)
	return r
}
/**
处理路由请求
**/
func dealRouter(r *gin.Engine){
     
	  r.GET("/",controller.Index{}.Index)
}