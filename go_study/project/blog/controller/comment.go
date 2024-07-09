package controller

import (
	"blog/models"
	"net/http"
	"strconv"
	"time"

	"github.com/gin-gonic/gin"
)
 
type CommentController struct {
}
/**
  添加数据的评论
**/
func (c CommentController) AddComment(r *gin.Context) {
      
	    user_id_str := r.PostForm("user_id")   //转换成整型
		user_id ,_ := strconv.Atoi(user_id_str)
		article_id_str := r.PostForm("article_id")
		article_id ,_ := strconv.Atoi(article_id_str)
		content := r.PostForm("content")
		add_time :=time.Now().UnixNano() / int64(time.Millisecond)
		parent_id_str := r.PostForm("parent_id")
		parent_id ,_ := strconv.Atoi(parent_id_str)
		comment := models.Comment{
			Article_id: article_id,
			Content:content,
			User_id:user_id,
			Add_time:int(add_time),
			Parent_id:parent_id,
		}
		num ,err :=models.CommentAdd(comment)
		if err !=nil {
			 r.JSON(http.StatusOK,gin.H{
				   "code":200,
				   "msg":"操作失败",
				   "data":"",
			 })
		}
		if num > 0 {
			r.JSON(http.StatusOK,gin.H{
				"code":200,
				"msg":"操作失败",
				"data":num,
		  })
		}else{
			r.JSON(http.StatusOK,gin.H{
				"code":200,
				"msg":"操作失败",
				"data":"",
		  })
		}
}