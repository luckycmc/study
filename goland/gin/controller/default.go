package controller

import (
       "github.com/gin-gonic/gin"
       "net/http"
       _"strings"
       "fmt"
)
//绑定相应的 方法  空结构体
type Default struct {
       Base
}

// 前台首页
func (this *Default) DefaultIndex(r *gin.Context) {
       
       name := r.Query("name") //query  接受 ？a=1这样的参数  param 接受的是路由参数 /a/b/name
       fmt.Println(name)
       if name == "18" {
              this.Base.Success(r,name)
              return
       }
       //this.Base.Success(r,name)
       //json字符串
      /* r.JSON(http.StatusOK, gin.H{
              "title": "zpw",
              "ce":    name,
              "score": 80,
              "hobby": []string{"吃饭", "睡觉", "写代码"},
       })*/
       //字符串
       //r.String(http.StatusOK, "index page")
       //页面
      /* r.HTML(http.StatusOK,"index.html",gin.H{
              "title": "zpw",
              "ce":    "cc",
              "score": 80,
              "hobby": []string{"吃饭", "睡觉", "写代码"},
       })*/
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

//使用map  输出json  数据
func(this *Default) DefaultJson(r *gin.Context){
       //可以直接map数据
       map_struct_data := map[string]string{
              "name":"zpw",
              "age":"18",
       }
       r.JSON(http.StatusOK,map_struct_data)
}