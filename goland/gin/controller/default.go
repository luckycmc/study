package controller

import (
       "github.com/gin-gonic/gin"
       "net/http"
)

type Default struct {
}

// 前台首页
func (this *Default) DefaultIndex(r *gin.Context) {
       
       //json字符串
       /*r.JSON(http.StatusOK, gin.H{
              "title": "zpw",
              "ce":    "cc",
              "score": 80,
              "hobby": []string{"吃饭", "睡觉", "写代码"},
       })*/
       //字符串
       //r.String(http.StatusOK, "index page")
       //页面
       r.HTML(http.StatusOK,"index.html",gin.H{
              "title": "zpw",
              "ce":    "cc",
              "score": 80,
              "hobby": []string{"吃饭", "睡觉", "写代码"},
       })
}
/**
 @首页列表
*/
func(this *Default) DefaultList(r *gin.Context){
       //r.String(http.StatusOK, "index list page....")
       //页面
       r.HTML(http.StatusOK,"list.html",gin.H{
              "title": "list",
              "ce":    "cclist",
              "score": 80,
              "hobby": []string{"吃饭1", "睡觉1", "写代码"},
       })
} 
