package main
import (
	"fmt"
	"os"
	"io"
)
//循环读取文件
func main(){
	filename := "./a.txt"
	//打开文件
	file,err := os.Open(filename)
	//查看是否打开成功
	if err != nil{
		fmt.Println("文件打开失败",err)
		return
	}
	defer file.Close()
	var content []byte
	var tmp = make([]byte,128)
	//判断文件是否读取完毕
	for{
		n,err := file.Read(tmp)
		if err == io.EOF{
			fmt.Println("文件去取完毕!")
			break
		}
		//文件读取书否有错误
		if err != nil{
			fmt.Println("文件读取有误!",err)
			 return
		}
		content = append(content,tmp[:n]...)
		//fmt.Println("******************************************************")
	}
	fmt.Println(string(content))
}