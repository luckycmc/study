package controller

import (
       "github.com/gin-gonic/gin"
       "net/http"
)

type Default struct {
}

// 前台首页
func (this *Default) DefaultIndex(r *gin.Context) {

       r.JSON(http.StatusOK, gin.H{
              "title": "zpw",
              "ce":    "cc",
              "score": 80,
              "hobby": []string{"吃饭", "睡觉", "写代码"},
       })
       //r.String(http.StatusOK, "index page")
}
