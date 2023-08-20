package controller

import (
    "github.com/gin-gonic/gin"
    "net/http"
)

type Admin struct {
    Base
}

// 后台页面
func (this *Admin) AdminIndex(r *gin.Context) {

    r.String(http.StatusOK, "this is admin module")
    //this.Base.Success(r)
}

// 后台列表
func (this *Admin) AdminList(r *gin.Context) {
    r.String(http.StatusOK, "this is admin AdminList")
}
