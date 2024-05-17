package router

import (
	"blog/controller/api"
	"net/http"

	"github.com/gin-gonic/gin"
)

func ApiRoutesInit(r *gin.Engine) {

	apiRoute := r.Group("/api")
	{
		//添加导航
		apiRoute.GET("/add", api.NavController{}.Save)

		//新闻页面
		apiRoute.GET("/news", func(ctx *gin.Context) {
			ctx.JSON(http.StatusOK, gin.H{
				"content":     "普京连人",
				"create_time": "2024-03-18",
			})
		})

	}
}
