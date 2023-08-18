package main

import (
	"net/http"
	"github.com/gin-gonic/gin"
   "strconv"
   "fmt"
)
//初始化操作
func init(){
   //启动数据库
   initDB()
}
//图书列表页
func bookListHandler(c *gin.Context){

	  
    bookList,err := queryAllBook()
     if err != nil {
        c.JSON(http.StatusOK,gin.H{
            "code": 1,
            "msg": err,
        })
        return 
     }
     c.HTML(http.StatusOK,"book/book_list.tmpl",gin.H{
           "code":0,
           "data":bookList,
     })
}
//显示插入页面
func newBookHandler(c *gin.Context){
     
     c.HTML(http.StatusOK,"book/new_book.html",nil)
     
}
//数据入库
func createBookHandler(c *gin.Context){
     //获取post的参数
     var msg string
     titleVal  := c.PostForm("title")
     priceVal  := c.PostForm("price")
     price,err := strconv.ParseFloat(priceVal,64)

     //参数校验
     if err != nil {
        msg :="无效参数" 
        c.JSON(http.StatusOK,gin.H{
              "msg":msg,
        })
        return
     }
     fmt.Printf("%T %T\n", titleVal, priceVal)
     //数据入库
     err = insertBook(titleVal,price)

     if err != nil{
         msg = "插入数据失败，请重试！"
         c.JSON(http.StatusOK,gin.H{
              "msg":msg,
         })
         return
     }
     //跳转到列表页
     c.Redirect(http.StatusMovedPermanently, "/book/list")
}
//web 路由
func webRouer() *gin.Engine{
	 //初始化路由
	r:= gin.Default()
    //加载对应的魔板
    r.LoadHTMLGlob("templates/**/*")
    /**********注册路由 start***************/
    //首页
    r.GET("/",func(c *gin.Context){
         c.String(http.StatusOK,"bcms is start...")
    }) 
    //图书俩列表页
    r.GET("/book/list",bookListHandler)
    //页面显示路由
    r.GET("/book/create",newBookHandler)
    r.GET("/book/new",newBookHandler)
    //获取对应的参数
    r.POST("/book/new",createBookHandler)
    /**********注册路由 end***************/
    return r;
}
func main(){
	
	 r := webRouer()
    //启动服务
	 r.Run()
}