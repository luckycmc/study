package main
import(
	"fmt"
)
//使用一个结构体管理队列
type Queue struct{
	maxSize int
	array [4]int   //数组=>模拟队列
	front int      //表示指向队首
	rear  int      // 表示指向队列尾部
}
//主函数
func main()  {
	
}