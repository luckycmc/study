package main

import (
	"example/gee"
	"net/http"
)

/*
*

	主要实现 gin web的简单路�? 和相应的参数的一个简单的框架

*
*/
func main() {
	//初始�?
	r := gee.New()
	//第一版的数据请求

	/*r.GET("/",func(w http.ResponseWriter, r *http.Request) {
			fmt.Fprintf(w, "URL.Path = %q\n", r.URL.Path)
		})

		r.GET("/hello",func(w http.ResponseWriter, r *http.Request) {
			  for k,v := range r.Header{
				fmt.Fprintf(w, "Header[%q] = %q\n", k, v)
			  }
		})
	   r.GET("/login",func(w http.ResponseWriter, r *http.Request) {})*/
	//  context 包含 res he response 的数据信�?
	//注册路由
	/* r.GET("/",func(c *gee.Context) {
		   c.HTML(http.StatusOK,"<h1>Hello Gee</h1>")
	})
	r.GET("/hello",func(c *gee.Context) {
		  c.String(http.StatusOK,"hello %s, you're at %s\n", c.Query("name"), c.Path)
	})
	//返回json
	r.GET("/login",func(c *gee.Context) {
		   c.Json(http.StatusOK,gee.H{
				"username": c.Query("username"),
				"password": c.Query("password"),
		   })
	})*/
	//带参数
	r.GET("/hello/:name", func(ctx *gee.Context) {

		ctx.String(http.StatusOK, "hello %s, you're at %s\n", ctx.Param("name"), ctx.Path)
	})
	//全路径
	r.GET("/assets/*filepath", func(c *gee.Context) {
		c.Json(http.StatusOK, gee.H{"filepath": c.Param("filepath")})
	})

	r.Run(":9999")

}
