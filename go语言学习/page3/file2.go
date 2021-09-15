package main
import (
	"fmt"
	"os"
	"io"
	"bufio"
)
//bufio安行读取文件
func main(){
	filename := "./a.txt"
    //打开文件
	file,err := os.Open(filename)
	//判断文件是否打开
	if err != nil{
		fmt.Println("文件打开有误!",err)
		return 
	}
	defer file.Close()
	//按行读取
	reader := bufio.NewReader(file)  //打开redis手柄
	for {
		line, err := reader.ReadString('\n') //注意是字符 //
		if err == io.EOF {
			fmt.Println("文件读完了")
			break
		}
		if err != nil {
			fmt.Println("read file failed, err:", err)
			return
		}
		fmt.Print(line)
	}
}
