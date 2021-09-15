package main
import (
	"fmt"
	"os"
)
func main(){
   //先打开一个文件,然后往里面写东西
   file,err := os.OpenFile("b.txt",os.O_CREATE|os.O_TRUNC|os.O_WRONLY, 0666)
   if err != nil {
	   fmt.Println("文件打开有误",err)
	   return 
   }
   defer file.Close()//关闭文件
   str := "hellow 沙河\n"
   file.Write([]byte(str))
   file.WriteString("hello 小王子")
}