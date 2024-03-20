package router

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func DefaultRoutesInit(r *gin.Engine) {
	defaultRoute := r.Group("/")
	{
		defaultRoute.GET("/", func(ctx *gin.Context) {
			ctx.String(http.StatusOK, "首页")
		})
	}
}
