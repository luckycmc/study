package gee

import (
	"net/http"
)

// HandlerFunc 是一个函数类型 参数是 *Context
type HandlerFunc func(*Context)

//定义结构体引擎
type Engine struct{
	router *router
}

//实例化结构体
func New() *Engine{
	return &Engine{router:newRouter()}
}

//添加路由
func(engine *Engine) addRoute(method string,pattern string,handler HandlerFunc){
        engine.router.addRoute(method,pattern,handler)
}

//定义GET方法
func(engine *Engine) GET(pattern string,handler HandlerFunc){
       
	    engine.addRoute("GET",pattern,handler)
}
//定义POST方法

func(engine *Engine) POST(pattern string,handler HandlerFunc){
	   engine.addRoute("POST",pattern,handler)
}
//启动服务
func (engine *Engine) Run(addr string) (err error) {
	return http.ListenAndServe(addr, engine)
}
//实现ServerHttp  type Handler interface 实现处理的函数 必须要有
func (engine *Engine) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	c := newContex(w,req)
	engine.router.handle(c)
}

