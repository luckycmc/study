package controller

import (
	"blog/models"
	"net/http"

	"github.com/gin-gonic/gin"
)

type IndexController struct{

}
/**
  bolg  首页
*/
func(con IndexController) Index(c *gin.Context) {
      page :=1
	  ArtList,_:= models.FindArticleWithPage(page)
      ArticleAll := models.QueryArticleRowNum()

	  c.JSON(http.StatusOK,gin.H{
		  "article":ArtList,
		  "articleAll":ArticleAll,
	  })
}

