package main

import (
    "github.com/gin-gonic/gin"
    //"net/http"
    "gin/router"
)

func main() {
    r := gin.Default()
    //加载静态页面
    r.LoadHTMLGlob("test_gin_html/*")
    //引入路由
    router.ApiRouterInit(r)
    //后台路由
    router.ApiAdminInit(r)
    //前台模块
    router.DefaultInit(r)
    //启动服务
    r.Run()
}
