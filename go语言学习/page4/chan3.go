package main
import (
	"fmt"
	"sync"
	//"runtime"
)
var x int64
var wg sync.WaitGroup
//var lock sync.Mutex
func add() {
	for i := 0; i < 5000; i++ {
		//lock.Lock()//枷锁
		  //runtime.Gosched()
		  x = x + 1
		//lock.Unlock()//解锁  
	}
	wg.Done()
}
func main() {
	wg.Add(2)
	go add()
	go add()
	wg.Wait()
	fmt.Println(x)
}