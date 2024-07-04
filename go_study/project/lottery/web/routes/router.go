package routes

import (
	"lottery/web/default/controller"
	kocp "lottery/web/kocp/controller"

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
      
	  //首页
	  r.GET("/",controller.Index{}.Index)
	  //抽奖页面
	  r.GET("/startLucky",controller.Index{}.StartLucky)
	  //处理抽奖的数据逻辑
	  r.GET("/lucky",controller.Lucky{}.Lucky)
     
	  //实现 抽奖的后台
	  admin := r.Group("/admin")
	  {
          admin.GET("/",kocp.User{}.Index)
		  //admin.GET("/user")
		  //admin.GET("/gift")
		  //admin.GET("/code")
		  //admin.GET("/result")
		  //admin.GET("/blackip")
	  }
	  
}