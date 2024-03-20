package router

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func sayHello(c *gin.Context) {

	c.JSON(200, gin.H{
		"message": "hello golang",
	})
}

// 首页
func index(c *gin.Context) {
	//获取get 参数
	aid := c.Query("aid")
	/*c.JSON(200, gin.H{
		"index":   "index page",
		"titles":  "这是一个好网站",
		"aid is ": aid,
	})*/
	c.HTML(http.StatusOK, "index.html", gin.H{
		"title": "this is first page",
		"aid":   aid,
	})
}

// 获取路由参数
func getForm(c *gin.Context) {

	aid := c.Query("aid")
	c.String(http.StatusOK, "aid=%s", aid)
}

// 获取动态路由
func getParams(c *gin.Context) {
	userId := c.Param("uid")
	c.String(http.StatusOK, "userId=%s", userId)
}

// 添加用户页面
func addUser(c *gin.Context) {

	c.HTML(http.StatusOK, "add_user.html", gin.H{})
}

// 处理注册逻辑
func doAddUser(c *gin.Context) {
	//获取form参数
	userName := c.PostForm("username")
	passWord := c.PostForm("password")
	//设置带默认值的 form表单
	age := c.DefaultPostForm("age", "20")
	c.JSON(http.StatusOK, gin.H{
		"username": userName,
		"password": passWord,
		"age":      age,
	})
}
func Start() {

	r := gin.Default() //返回默认的路由引擎
	//加载静态文件 单一的  如果分目录需要 r.LoadHTMLGlob("templates/**/*")
	r.Static("/static", "./static") //加载静态资源
	r.LoadHTMLGlob("templates/*")
	//注册Api模块路由
	ApiRoutesInit(r)
	//注册default模块
	DefaultRoutesInit(r)
	//注册admin 模块
	AdminRoutesInit(r)
	//r.GET("/", index)
	//r.GET("/hello", sayHello)
	//r.GET("/getForm", getForm)
	//获取动态路由
	//r.GET("/getParams/:uid", getParams)
	//注册页面
	//r.GET("/addUser", addUser)
	//处理注册逻辑
	//r.POST("/doAddUser", doAddUser)
	//启动服务
	r.Run() //默认是8080
	//r.Run(":9090")// 我们可以改为9090
	//r.Run()
}
