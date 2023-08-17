package main

import (
    "github.com/gin-gonic/gin"
    "net/http"
)
type Data struct{
    Name string
    Age int
}
//设置路由
func setupRouter() *gin.Engine {
    data := &Data{
        Name:"zpw",
        Age:20,
    }
    r := gin.Default()
    //加载模板目录
    r.LoadHTMLGlob("test_gin_html/*")
    //json 的形式返回数据
    r.GET("/ping", func(c *gin.Context) {
        c.JSON(http.StatusOK, gin.H{
             "code":"200",
             "msg":"success",
             "data":data,
        })
    })
    //静态输出数据
    r.GET("/",func(c *gin.Context){
        
         c.String(http.StatusOK,"hello gin")
    })
    //静态页面的处理
    r.GET("/index",func(c *gin.Context){
         
          c.HTML(http.StatusOK,"index.html",gin.H{
              "title":"zpw",
              "ce":"cc",
           })
    })
    return r
}

func main() {
    r := setupRouter()
    r.Run(":8000") // listen and serve on 0.0.0.0:8080
}