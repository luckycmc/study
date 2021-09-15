package main
import  (
	"fmt"
	"time"
)

func main(){
	ch := make(chan int,1)//定影一个缓存管道
	for{
		select{
			case ch <- 0 :
			case ch <- 1 :	
		}
		i := <-ch
		time.Sleep(time.Second)
        fmt.Println("Value received:", i)
	}
}