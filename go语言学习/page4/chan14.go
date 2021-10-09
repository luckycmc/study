package main
import (
	"fmt"
)

func main() {

	 var mych chan int

	 //初始化管道
	 mych = make(chan int,3)  //缓存区为3

	 // 注意点: 管道中只能存放声明的数据类型, 不能存放其它数据类型
     mych <- 3

	 fmt.Println("111")

	 // 注意点: 如果管道中数据已满, 再写入就会报错
	mych<- 666
	mych<- 777
}