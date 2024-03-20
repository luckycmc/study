package router

import (
	"blog/controller/admin"

	"github.com/gin-gonic/gin"
)

func AdminRoutesInit(r *gin.Engine) {

	adminRoute := r.Group("/admin")
	{
		//用户
		adminRoute.GET("/user", admin.UserController{}.Index)
		//添加用户
		adminRoute.GET("/user/add", admin.UserController{}.Add)
		//新闻首页
		adminRoute.GET("/news", admin.NewsController{}.Index)
		//测试新闻页面
		adminRoute.GET("/success", admin.NewsController{}.Test)
	}
}
