package main
import(
	"fmt"
	"os"
	//"time"
	"../process"
)

//定义连个变量 一个表示用户id，一个表示用户密码
var userId int
var passPwd string
var userName string

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
			fmt.Println("请输入用户id")
			fmt.Scanf("%d\n",&userId)
			fmt.Println("请输入用户密码")
			fmt.Scanf("%s\n",&passPwd)
			//完成登录
		    //1.创建一个UserProcess的实例
			up := &process.UserProcess{

			}
			up.Login(userId,passPwd)
		  case 2:
			fmt.Println("注册用户")
			fmt.Println("请输入用户id")
			fmt.Scanf("%d\n",&userId)
			fmt.Println("请输入用户密码")
			fmt.Scanf("%s\n",&passPwd)
			fmt.Println("请输入用户的名称(nickname)")
			fmt.Scanf("%s\n",&userName)
			//1.创建一个UserProcess的实例 完成用户的注册请求
			up := &process.UserProcess{

			}
			up.Register(userId,passPwd,userName)
		  case 3:
			fmt.Println("退出系统")
			os.Exit(0)
		  default:
			fmt.Println("你的输入有误请重新输入对应的数字")
		  }
	 }
	 //time.Sleep(time.Second *10)
}