package router

import (
	"blog/controller"
	"blog/middleware"

	"github.com/gin-gonic/gin"
)

func InitRouter() *gin.Engine {
	//创建默认的引擎
	router := gin.Default()
	//注册静态文件
	router.LoadHTMLGlob("views/**/*")
	//注册路由
	registerRouter(router)
	return router
}

// 路由注册
func registerRouter(r *gin.Engine) {

	//首页
	r.GET("/", controller.IndexController{}.Index)
	//注册页路由
	r.GET("/register", controller.RegisterController{}.RegisterGet)
	//用户数据注册
	r.POST("/register", controller.RegisterController{}.RegisterPost)

	/**登录 start**/
	r.GET("/login", controller.LoginController{}.LoginGet)
	r.POST("/login", controller.LoginController{}.LoginPost)
	/**登录 end**/

	/***文章相关的路由 start**/
	r.GET("/article/add", controller.ArticleController{}.AddArticleGet)
	r.POST("/article/add", controller.ArticleController{}.AddArticlePost)
	r.GET("/article/show/:id", controller.ArticleController{}.ArticleDetails)
	/***文章相关的路由 end**/


	/***评论相关的 路由 start***/
     comment := r.Group("/comment",middleware.CheckToken)
	 {
         comment.POST("/add",controller.CommentController{}.AddComment)
	 }
	/***评论相关的 路由 start***/

}
