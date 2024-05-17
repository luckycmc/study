package cms

import (
	"github.com/gin-gonic/gin"
	"net/http"
)

type Book struct{

}
//创建视图文件
func(this *Book) CreateBook(c *gin.Context){
	
	c.HTML(http.StatusOK,"book/new_book.html",nil) 
}