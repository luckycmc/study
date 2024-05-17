package api

import (
	"blog/models"

	"github.com/gin-gonic/gin"
)

type NavController struct {
}

// 添加导航
func (c NavController) Save(ctx *gin.Context) {
	navName := ctx.Query("nav")
	nav := models.Nav{Name: navName}
	result := models.DB.Create(&nav)
	var resultStr string
	if result.RowsAffected > 0 {
		resultStr = "success"
	} else {
		resultStr = "fail"
	}
	ctx.String(200, resultStr)
}

// 获取所有的列表
func (c NavController) Index(ctx *gin.Context) {

}
