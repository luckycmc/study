package api

import (
	_ "fmt"
	"github.com/gin-gonic/gin"
	"gormlearn/model"
	"time"
)

//新增用户
func SaveUser(c *gin.Context) {

	user := &model.User{
		Username:   "zhangsan",
		Password:   "123e4r5",
		CreateTime: time.Now().UnixMilli(),
	}
	model.SaveUser(user)
	c.JSON(200, user)
}

//获取用户信息
func GetUser(c *gin.Context){
      user := model.GetById(1)
	  c.JSON(200,user)
}

//获取所有用户信息

func GetAll(c *gin.Context){
     
	  users := model.GetAll()
	  c.JSON(200,users)
}
