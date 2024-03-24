package admin

import (
	"blog/models"
	"fmt"
	"net/http"
	"strconv"

	"github.com/gin-gonic/gin"
)

type UserController struct {
	BaseController
}

// 按照用户名查找
func (c UserController) Find(ctx *gin.Context) {
	name := ctx.Query("name") //获取浏览器参数
	users := []models.User{}
	models.DB.Where("username=?", name).Find(&users)
	ctx.JSON(http.StatusOK, gin.H{
		"status": 200,
		"result": users,
	})
}

// 用户首页
func (c UserController) Index(ctx *gin.Context) {

	users := []models.User{}
	models.DB.Find(&users)
	ctx.JSON(http.StatusOK, gin.H{
		"status": 200,
		"result": users,
	})
	//ctx.String(http.StatusOK, "这是用户首页")
}

// 新增用户
func (c UserController) Add(ctx *gin.Context) {
	user := models.User{
		Username: "zpw",
		Age:      18,
		Sex:      30,
	}
	//添加数据
	result := models.DB.Create(&user)
	if result.RowsAffected > 1 {
		fmt.Print(user.Id)
	}
	fmt.Println(result.RowsAffected)
	fmt.Println(user.Id)
	ctx.String(http.StatusOK, " add 增加用户 success")
}

// 修改用户数据
func (c UserController) Update(ctx *gin.Context) {
	user := models.User{Id: 7}
	models.DB.Find(&user)
	user.Username = "zpw100"
	result := models.DB.Save(&user)
	//fmt.Println(result.RowsAffected)
	if result.RowsAffected < 1 {
		ctx.String(200, "更新失败")
	} else {
		ctx.String(200, "更新成功")
	}
}

// 删除数据
func (c UserController) Delete(ctx *gin.Context) {
	id := ctx.Query("id")
	if id == "" {
		ctx.String(200, "操作有误!")
	}
	paramId, _ := strconv.ParseInt(id, 10, 64)
	user := models.User{Id: int(paramId)}
	result := models.DB.Delete(&user)
	fmt.Println(result.RowsAffected)
}
