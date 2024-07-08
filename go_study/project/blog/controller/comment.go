package controller

import (
	"blog/models"

	"github.com/gin-gonic/gin"
)

type CommentController struct {
}
/**
  添加数据的评论
**/
func (c CommentController) AddComment(r *gin.Context) {
      
	    user_id := r.PostForm("user_id")
		article_id := r.PostForm("article_id")
		content := r.PostForm("content")
		add_time := r.PostForm("add_time")
		parent_id := r.PostForm("parent_id")
		comment := models.Comment{
			Article_id: article_id,
			Content:content,
			User_id:user_id,
			Add_time:add_time,
			Parent_id:parent_id,
		}
		models.CommentAdd(comment)
}