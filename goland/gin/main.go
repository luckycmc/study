package main

import (
    "github.com/gin-gonic/gin"
    //"net/http"
    "gin/router"
)

func main(){
    r:= gin.Default()
    //引入路由
    router.ApiRouterInit(r)
    router.ApiAdminInit(r)
    r.Run()
}
