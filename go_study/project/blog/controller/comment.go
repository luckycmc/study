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
		//获取当前时间戳
		add_time :=time.Now().Unix()
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
				"msg":"操作成功",
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

/**
  @根据文章的id 获取相应的评论
**/
func(c CommentController) GetCommentList(r *gin.Context){
        
	     article_id_param := r.Query("article_id")
		 if article_id_param =="" {
			 r.JSON(http.StatusOK,gin.H{
                    "code":200,
					"msg":"article_id 参数不能为空",
					"data":"",
			 })
			 return
		 }
		 article_id,_ := strconv.Atoi(article_id_param)
		 var page int
		 page_param := r.Query("page")
		 // page 默认为 1
		 if page_param == "" {
			   page = 1
		 }else{
			page,_ = strconv.Atoi(page_param)
		 }
		 data,err := models.GetCommentByArticleId(article_id,page)
		 if err != nil {
				r.JSON(http.StatusOK,gin.H{
					"code":200,
					"msg":"操作失败",
					"data":"",
			})
			return
		 }
		 //返回相应的处理结果
		 r.JSON(http.StatusOK,gin.H{
				"code":200,
				"msg":"success",
				"data":data,
	     })
}

//获取当前文章下的所有分类

func(c CommentController) GetCommentAllList(r *gin.Context){
	   
	   article_id_form := r.Query("article_id")
	   if article_id_form == "" {
		     
		    r.JSON(http.StatusOK,gin.H{
				"code":200,
				"msg":"",
				"data":nil,
			})
	   }
	   // MA201013000
	   //转换整形
	   article_id,_:= strconv.Atoi(article_id_form)
	   //获取分页
	   page_form := r.Query("page")
	   var page int
	   if page_form =="" {
		  page =1
	   }else{
		page,_ = strconv.Atoi(page_form)
	   }
	   data,err := models.GetAllCommentByArticleId(article_id,page)
	   if err != nil {
		   r.JSON(http.StatusOK,gin.H{
			    "code":200,
				"msg":"操作有误!",
				"data":nil,
		   })
		   return
	   }
       // 返回相应的数据
	   r.JSON(http.StatusOK,gin.H{
		"code":200,
		"msg":"success!",
		"data":data,
       })
}