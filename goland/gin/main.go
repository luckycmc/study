package main

import (
    "github.com/gin-gonic/gin"
    //"net/http"
    "gin/router"
)

func main() {
    r := gin.Default()
    //引入路由
    router.ApiRouterInit(r)
    //后台路由
    router.ApiAdminInit(r)
    //启动服务
    r.Run()
}
