package main
import (
	"fmt"
	"runtime"
	"time"
)
func a() {
	for i := 1; i < 10; i++ {
		time.Sleep(time.Second)
		fmt.Println("A:", i)
	}
}

func b() {
	for i := 1; i < 10; i++ {
		time.Sleep(time.Second)
		fmt.Println("B:", i)
	}
}
func main(){
   runtime.GOMAXPROCS(2)//设置cpu的核数
   go a()
   go b()
   for{

   }
}