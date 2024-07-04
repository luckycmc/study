package controller

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

type Index struct {
}
//首页
func (c Index) Index(r *gin.Context) {
        
	 r.HTML(http.StatusOK,"index/index.html",gin.H{
		 "ok":"ok",
	 })
}
//抽奖页面
func(c Index) StartLucky(r *gin.Context){
     r.HTML(http.StatusOK,"index/lucky.html",nil)
}