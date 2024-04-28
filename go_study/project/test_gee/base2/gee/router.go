package gee

import (
	"fmt"
	"log"
	"net/http"
	"strings"
)

type router struct {
	roots map[string]*node
	handlers map[string]HandlerFunc
}

func newRouter() *router {
	return &router{
		roots: make(map[string]*node),
		handlers: make(map[string]HandlerFunc),
	}
}

// Only one * is allowed   返回的是字符串切片
func parsePattern(pattern string) []string{

	 vs := strings.Split(pattern,"/")
	 parts := make([]string,0)
 
     for _,item := range vs{
		 if item != "" {
			  parts =append(parts, item)
			  // *后面是全量匹配
			  if item[0] == '*' {
				  break
			  }
		 }
	 }

	 return parts
}
//添加路由
func (r *router) addRoute(method string, pattern string, handler HandlerFunc) {

	log.Printf("Route %4s - %s", method, pattern)
	//解析路由
	parts := parsePattern(pattern)
   
	key := method + "-" + pattern
	_, ok := r.roots[method]
	if !ok {
		r.roots[method] = &node{}
	}
	r.roots[method].insert(pattern, parts, 0)
	r.handlers[key] = handler
} 
//获取相应的路由
func(r *router) getRoute(method string,path string)(*node ,map[string]string){
    
	searchParts := parsePattern(path)
	params := make(map[string]string)
	root, ok := r.roots[method]
 
	if !ok {
		return nil, nil
	}
	fmt.Println(searchParts)
	n := root.search(searchParts, 0)
	if n != nil {
		parts := parsePattern(n.pattern)
		for index, part := range parts {
			//获取相应的参数
			if part[0] == ':' {
				params[part[1:]] = searchParts[index]
			}
			//全量匹配
			if part[0] == '*' && len(part) > 1 {       
				params[part[1:]] = strings.Join(searchParts[index:], "/")
				break
			}
		}
		return n, params
	}
	return nil,nil
}    
//执行对应的回调函数
func (r *router) handle(c *Context) {
	//获取相应的路由
	n, params := r.getRoute(c.Method, c.Path)
	if n != nil {
		c.Params = params
		key := c.Method + "-" + n.pattern
		//执行函数
		r.handlers[key](c)
	} else {
		c.String(http.StatusNotFound, "404 NOT FOUND: %s\n", c.Path)
	}
}