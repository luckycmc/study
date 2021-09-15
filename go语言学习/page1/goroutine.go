package main
import (
	"fmt"
	"time"
)
func running(){
	var times int
	for{
		times++
		fmt.Println("tick",times)
		//延时1秒
		time.Sleep(time.Second)
	}
}
func main(){
	//并发执行程序
	go running()
	//接受命令输入,不多任何事情
	var input string
	fmt.Scanln(&input)
}