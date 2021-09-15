package main
import "fmt"
func funcA() {
	fmt.Println("func A")
}

func funcB() {
	defer func() {  //通过recevoer 函数将数据恢复正常
		err := recover()
		//如果程序出出现了panic错误,可以通过recover恢复过来
		if err != nil {
			fmt.Println("recover in B pinic")
		}
	}()
	panic("panic in B") //panic导致程序崩溃，异常退出了。
}

func funcC() {
	fmt.Println("func C")
}
func main() {
	funcA()
	funcB()
	funcC()
}