package main
import (
	"fmt"
	"net/http"
)
//转发sayhello
func sayHello(w http.ResponseWriter,r *http.Request){
  fmt.Fprintf(w,"hello 沙河！")
}
//主函数
func main(){
	//pipei/转发给sayHello函数

   http.HandleFunc("/",sayHello)
   err := http.ListenAndServe("127.0.0.1:9090",nil)
   if err != nil{
	   fmt.Println("http server failed,err:%v",err)
	   return
   }
}