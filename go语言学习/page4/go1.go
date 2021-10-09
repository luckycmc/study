package main
import (
	"fmt"
	"time"
)
//唱歌
func sing()  {
	for i:=0; i< 10; i++{
	 fmt.Println("我在唱歌")
	 time.Sleep(time.Millisecond)
	}
}
//跳舞
func dance() {
	for i:=0; i< 10; i++{
	 fmt.Println("我在跳舞---")
	 time.Sleep(time.Millisecond)
	}
}

func main() {
   // 串行: 必须先唱完歌才能跳舞
	//sing()
	//dance()

	// // 并行: 可以边唱歌, 边跳舞
    go sing();  //开启两个协成
	go dance(); //开启两个协成  数据打印交替执行

	for{

	}
}