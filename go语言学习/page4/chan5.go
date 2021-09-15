package main
import (
	"fmt"
	"sync"
	"time"
)
//读写锁适合读多写少的情况,假设差不多也可以用互斥锁
var (
	x      int64
	wg     sync.WaitGroup
	lock   sync.Mutex   //互斥锁
	rwlock sync.RWMutex //读写锁
)

func write() {
	 lock.Lock()   // 加互斥锁
	//rwlock.Lock() // 加写锁
	x = x + 1
	time.Sleep(10 * time.Millisecond) // 假设读操作耗时10毫秒
	//rwlock.Unlock()                   // 解写锁
	 lock.Unlock()                     // 解互斥锁
	wg.Done()
}

func read() {
	 lock.Lock()                  // 加互斥锁
	//rwlock.RLock()               // 加读锁
	time.Sleep(time.Millisecond) // 假设读操作耗时1毫秒
	//rwlock.RUnlock()             // 解读锁
	 lock.Unlock()                // 解互斥锁
	wg.Done()
}

func main() {
	start := time.Now()
	for i := 0; i < 10; i++ {
		wg.Add(1)
		go write()
	}

	for i := 0; i < 1000; i++ {
		wg.Add(1)
		go read()
	}

	wg.Wait()
	end := time.Now()
	fmt.Println(end.Sub(start))
}