package main
import (
	"fmt"
	"runtime"
)
//唱歌
func sing() {
   for i :=0;i<10;i++ {
	   fmt.Println("我在唱歌")
	   // Gosched使当前go程放弃处理器，以让其它go程运行。
       // 它不会挂起当前go程，因此当前go程未来会恢复执行
	   runtime.Gosched()  //让出当前协成供其他协成执行
   }
}
//跳舞
func dance() {
    
	for i := 0; i < 10; i++ {
		fmt.Println("我在跳舞....")
		 runtime.Gosched()
	}
}

func main() {
    
	  //启动两个协成
	  go dance()
	  go sing()

	  for{

	  }
}