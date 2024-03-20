package admin

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

type NewsController struct {
	BaseController
}

func (c NewsController) Index(ctx *gin.Context) {

	ctx.String(http.StatusOK, "新闻首页")
}

// 测试成功页面
func (c NewsController) Test(ctx *gin.Context) {
	c.Success(ctx)
}
