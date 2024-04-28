package controller

import (
	"blog/models"
	"blog/utils"
	"fmt"
	"net/http"

	"github.com/gin-contrib/sessions"
	"github.com/gin-gonic/gin"
)

type LoginController struct{

}
//登录页面
func(con LoginController) LoginGet(c *gin.Context) {
      
	   c.HTML(http.StatusOK,"login.html",gin.H{
		   "title": "登录页",
	   })
}
//登录逻辑
func(con LoginController) LoginPost(c *gin.Context){
      
	  //获取表单信息
      username := c.PostForm("username")
	  password := c.PostForm("password")

	  fmt.Println("username:",username,"password:",password)
	  //查询数据是否存在
	  id := models.QueryUserWithParam(username,utils.MD5(password))
	  fmt.Println("id:",id)
	  if id >0 {
		session := sessions.Default(c)
		session.Set("loginuser",username)
		session.Save() //保存session
		
		c.JSON(http.StatusOK,gin.H{"code":0,"message":"登录成功!"})
	  }else {
		c.JSON(http.StatusOK,gin.H{"code":0,"message":"登录失败"})
	  }
}        