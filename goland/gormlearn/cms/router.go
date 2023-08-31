package cms

import (
	"github.com/gin-gonic/gin"
)

//注册路由
func RegisterRouter(r *gin.Engine){
       book := &Book{
		
	   }
	   //图书的添加
	   r.GET("/book/new",book.CreateBook)
	   //图书修改
	   //图书列表
	   //图书删除
}