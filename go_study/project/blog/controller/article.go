package controller

import (
	"blog/models"
	"fmt"
	"net/http"
	"strconv"
	"time"

	"github.com/gin-gonic/gin"
)

type ArticleController struct{

}
//文章显示页面
func(con ArticleController) AddArticleGet(c *gin.Context) {
     
	    c.HTML(http.StatusOK,"default/write_article.html",gin.H{})
}
//处理form  数据交互
func(con ArticleController) AddArticlePost(c *gin.Context) {
	  
	   title := c.PostForm("title")
	   tags := c.PostForm("tags")
	   short := c.PostForm("short")
	   content := c.PostForm("content")

	   fmt.Printf("title:%s,tags:%s\n", title, tags)

	   art := models.Article{0, title, tags, short, content,"孔子学院", time.Now().Unix()}
       //数据入库
       _,err := models.AddArticle(art)
	   //返回数据给浏览器
	   response := gin.H{}

	   if err == nil {
		   //无误
		   response = gin.H{"code": 1, "message": "ok"}
	   } else {
		   response = gin.H{"code": 0, "message": "error"}
	   }
   
	   c.JSON(http.StatusOK, response)
}


//文章详情页
func(con ArticleController) ArticleDetails(c *gin.Context){
     
	   //获取参数id
	   idStr := c.Param("id")
	   if idStr =="" {
		   c.JSON(http.StatusOK,gin.H{
			     "data":"param is empty",
				 "code":-1,
		   })
		   return
	   }
	   //转化整形
	   id,_:=strconv.Atoi(idStr)
	   //获取文章id对应的信息
	   art := models.QueryArticleDetails(id)
	   //数据为空
	   if art.Title == "" {
			c.JSON(http.StatusOK,gin.H{
				"artData":"data is empty",
				"code":0,
		   })
		   return
	   }
	   c.JSON(http.StatusOK,gin.H{
		    "artData":art,
			"code":1,
	   })
}