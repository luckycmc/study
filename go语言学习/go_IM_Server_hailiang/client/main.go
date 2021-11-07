package main
import(
	"fmt"
	"os"
	"time"
)

//定义连个变量 一个表示用户id，一个表示用户密码
var userId int
var passPwd string

func main()  {
	 
	 //接受用户选着
	 var key int
	 //判断是否还继续选着菜单
	 var loop  = true
	 //循环接受客户端的请求
	 for loop{
        
		  fmt.Println("-----------------欢迎登陆多人聊天系统----------")
		  fmt.Println("\t\t\t 1 登陆聊天室")
		  fmt.Println("\t\t\t 2 注册用户")
		  fmt.Println("\t\t\t 3 退出系统")
		  fmt.Println("\t\t\t 3 请选择(1-3):")

		  fmt.Scanf("%d\n",&key)

		  switch key {
		  case 1:
			fmt.Println(" 登陆聊天室")
			loop = false
		  case 2:
			fmt.Println("注册用户")
			loop = false
		  case 3:
			fmt.Println("退出系统")
			os.Exit(0)
		  default:
			fmt.Println("你的输入有误请重新输入")
		  }
	 }

	 if key ==1 {
		 //说明用户要登录了
		 fmt.Println("请输入用户id")
		 fmt.Scanf("%d\n",&userId)
		 fmt.Println("请输入用户密码")
		 fmt.Scanf("%s\n",&passPwd)
		 //验证用户登陆
		 err := login(userId,passPwd)
		 if err != nil{
			 fmt.Println("用户登陆失败")
		 }else{
			 fmt.Println("登陆成功")
		 }
	 }else if key ==2 {
		fmt.Println("注册用户")
	 }


	 //time.Sleep(time.Second *10)
}