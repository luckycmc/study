package main

import (
	"fmt"
	"net/http"
)

func main() {

	  //路由回调函数
	  http.HandleFunc("/", myHandler)

	  //http 服务器监听
	  http.ListenAndServe("0.0.0.0:8080",nil)
}
//对应的回调函数
func myHandler(w http.ResponseWriter, r *http.Request) {
      
	fmt.Println(r.RemoteAddr, "连接成功")
    // 请求方式：GET POST DELETE PUT UPDATE
    fmt.Println("method:", r.Method)
    // /go
    fmt.Println("url:", r.URL.Path)
    fmt.Println("header:", r.Header)
    fmt.Println("body:", r.Body)
    // 回复
    w.Write([]byte("www.5lmh.com"))  //数据返回给客户端
}