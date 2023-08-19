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
//修改数据
func editBookHandler(c *gin.Context){
    
      //获取用户传递的id
      idStr := c.Query("id")
      //判断参数   
      if len(idStr) == 0{
          //没有携带参数是无效请求
          c.String(http.StatusOK,"请求参数有问题")
          return
      }
      //数据转换类型
      bookId ,err:=strconv.ParseInt(idStr,10,64)
      if err != nil {
         c.String(http.StatusOK,"没有携带参数则是无效请求")
         return 
      }
      //post参数请求
      if c.Request.Method == "POST"{
              
              //获取用户提交的数据
              title := c.PostForm("title")
              price := c.PostForm("price")
              priceVal ,err := strconv.ParseFloat(price,64)
              if err != nil {
                  c.String(http.StatusBadRequest, "无效的价格信息")
                  return 
              }
              // 2. 去数据库更新对应的书籍数据
              // ? id去哪儿了？
              err = editBook(title,priceVal,bookId)
              if err != nil {
                 c.String(http.StatusInternalServerError,"数据更新失败")
                 return 
              }
              c.Redirect(http.StatusMovedPermanently,"/book/list")

      }else{ //加载页面
            
            //获取数据给旧的魔板渲染
         bookObj, err := queryBookByID(bookId)
         if err != nil {
            c.String(http.StatusBadRequest, "无效的书籍id")
            return
         }
         // 3. 把书籍数据渲染到页面上
         c.HTML(http.StatusOK, "book/book_edit.html", bookObj)
      }
}
//数据删除
func deleteBookHandler(c *gin.Context){
      
       //获取参数
       id := c.Query("id")
       if len(id) == 0{
           //请求参数有问题
           c.String(http.StatusOK,"参数有问题")
           return
       }
       idVal,err := strconv.ParseInt(id,10,64)
       if err != nil {
             c.JSON(http.StatusOK, gin.H{
                  "code": 1,
                  "msg": err,
            })
         return
       }
       //删除数据
       err = deleteBook(idVal)
       if err != nil {
         c.JSON(http.StatusOK,gin.H{
              "code":0,
              "err":err,
         })
          return 
       }
       c.Redirect(http.StatusMovedPermanently,"/book/list")

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
    //编辑修改
    r.Any("/book/edit", editBookHandler)
    //数据删除
    r.GET("/book/delete", deleteBookHandler)
    /**********注册路由 end***************/
    return r;
}
func main(){
	
	 r := webRouer()
    //启动服务
	 r.Run()
}