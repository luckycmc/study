package gee

import (
	"encoding/json"
	"fmt"
	"net/http"
)

//key  是 string 类型 value 可以是go的任何数据类型
type H map[string]interface{}
// 上下文结构体
type Context struct {
	Writer http.ResponseWriter
	Req     *http.Request
	//request info
	Path string
	Method string
	Params map[string]string
	//response info
	StatusCode int
}
//创建上下文对象
func newContex(w http.ResponseWriter,req *http.Request) *Context{
	 return &Context{
		Writer: w,
		Req: req,
		Path: req.URL.Path,
		Method: req.Method,
	 }
}
//获取表单的值
func(c *Context) PostForm(key string) string{
	return c.Req.FormValue(key)
}
// ?name=zpw
func(c *Context) Query(key string) string{
   
	 return c.Req.URL.Query().Get(key)
}  
//获取参数的值
func(c *Context) Param(key string) string{
	 value,_ := c.Params[key]
	 return value
}
//设置相应的状态
func(c *Context) Status(code int){
	  c.StatusCode = code
	  c.Writer.WriteHeader(code)
}
//设置返回头
func(c *Context) SetHeader(key string,value string){
    
	   c.Writer.Header().Set(key,value)
}

//返回string  类型
func(c *Context) String(code int,format string,values ...interface{}){
	c.SetHeader("Content-Type","text/plain")
	c.Status(code)
	c.Writer.Write([]byte(fmt.Sprintf(format,values...)))
}

//返回json类型
func(c *Context) Json(code int,obj interface{}){
	c.SetHeader("Content-Type", "application/json")
	c.Status(code)
	encoder := json.NewEncoder(c.Writer)
	if err := encoder.Encode(obj); err != nil {
		http.Error(c.Writer, err.Error(), 500)
	}
}
//直接返回数据
func (c *Context) Data(code int, data []byte) {
	c.Status(code)
	c.Writer.Write(data)
}
//html文本
func (c *Context) HTML(code int, html string) {
	c.SetHeader("Content-Type", "text/html") // 文本类型
	c.Status(code) //状态
	c.Writer.Write([]byte(html)) // 把数据输出到客户端
}