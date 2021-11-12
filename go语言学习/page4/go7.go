package main
import(
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func hello(i int){
   defer wg.Done()  // goroutine结束就登记-1
   fmt.Println("Hello goroutine",i)
}

func main(){
	for i:=0;i<10;i++{
		wg.Add(1)  //启动一个goroutine 就登记加一
		go hello(i)
	}
	wg.Wait() //等待所有登记的goroutine 都结束
}