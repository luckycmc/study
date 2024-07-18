package middleware

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func CheckToken(r *gin.Context) {
      
	   token := r.Query("token")
	   
	   if token =="" {
		   
		    r.JSON(http.StatusOK,gin.H{
				  "code":1,
				  "msg":"没有token",
				  "data":"",
			})
			//c.Abort()终止后续的中间件执行 Abort 后续的东西不在执行 r.next（）执行下一个中间件
			r.Abort()
			return 
	   }
	  
	   //查询数据库 检验token是否存在
	   r.Next()
}