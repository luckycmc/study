package main
import (
	"fmt"
	"io/ioutil"
)
func main(){
	filename := "./a.txt"
	content,err := ioutil.ReadFile(filename)
	if err != nil{
		fmt.Println("文件读取错误")
		return 
	}
	fmt.Println(string(content)) //转换字符出输出
}