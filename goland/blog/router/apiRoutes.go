package router

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func ApiRoutesInit(r *gin.Engine) {

	apiRoute := r.Group("/api")
	{
		//用户
		apiRoute.GET("/user", func(ctx *gin.Context) {
			ctx.JSON(http.StatusOK, gin.H{
				"name": "张三",
				"age":  20,
			})
		})

		//新闻页面
		apiRoute.GET("/news", func(ctx *gin.Context) {
			ctx.JSON(http.StatusOK, gin.H{
				"content":     "普京连人",
				"create_time": "2024-03-18",
			})
		})

	}
}
