package main
import (
	"fmt"
	"os"
	"io"
)
func main(){
	//只读的方式打开文件下a.txt
	file,err := os.Open("./a.txt")
	if err != nil{
		fmt.Println("open file failed! err:",err)
		return 
	}else{
		fmt.Println("文件打开成功！正在读取文件....")
	}
	//关闭文件
	defer file.Close()  
	//使用Read方法读取文件数据
	var tmp = make([]byte,128)
	n,err := file.Read(tmp)
	if err == io.EOF {
		fmt.Println("文件读取完毕!")
		return
	}
	if err != nil{
		fmt.Println("read file failed ,err",err)
		return
	}
	fmt.Printf("读取了%d字节数据",n)
	fmt.Println(string(tmp[:n]))
}
