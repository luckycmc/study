package middleware

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func CheckToken(r *gin.Context) {
      
	   token := r.PostForm("token")
	   if token ==" " {
		   
		    r.JSON(http.StatusOK,gin.H{
				  "code":1,
				  "msg":"没有token",
				  "data":"",
			})
			return 
	   }
	  
	   //查询数据库 检验token是否存在
	   r.Next()
}