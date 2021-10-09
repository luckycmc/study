package main
import (
 "fmt"
 "time"
)
func main() {
 // 1.创建一个周期定时器
 ticker := time.NewTicker(time.Second)
 // 2.不断从重启定时器中获取时间
 for{
  t := <-ticker.C // 系统写入数据之前会阻塞
  fmt.Println(t)
 }
}