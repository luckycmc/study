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
		//用户名称查找
		adminRoute.GET("/find", admin.UserController{}.Find)
		//修改用户
		adminRoute.GET("/save", admin.UserController{}.Update)
		//删除用户
		adminRoute.GET("/del", admin.UserController{}.Delete)
		//添加用户
		adminRoute.GET("/user/add", admin.UserController{}.Add)
		//新闻首页
		adminRoute.GET("/news", admin.NewsController{}.Index)
		//测试新闻页面
		adminRoute.GET("/success", admin.NewsController{}.Test)
	}
}
