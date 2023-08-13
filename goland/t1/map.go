package main
import(
	"fmt"
)
//map是一种无顺的key-value数据结构,Go语言的map是引用类型,必须初始化
// 相当于 php 的 关联数组
func main(){
     //语法格式map[keyType][value] 
     //1.基本的使用
     scoreMap := make(map[string]int,8)
     scoreMap["张三"] = 90
     scoreMap["李四"] = 100
     fmt.Println(scoreMap)
     fmt.Println(scoreMap["张三"])
     fmt.Printf("type is a:%T\n",scoreMap)
     fmt.Println("-------------------------------1")
     //2.map也支持在声明的时候填充元素
     userInfo :=map[string]string{
     	"username":"zpw",
     	"pwd":"123456",
     }
     fmt.Println(userInfo)
     fmt.Println("-------------------------------2")
     //3.判断某个key是否存在
     v,ok := scoreMap["张三"]
     if ok{
     	fmt.Println(v)
     }else{
     	fmt.Println("查无此人")
     }
     fmt.Println("-------------------------------3")
     //便利数组
     for k,v := range scoreMap{
     	fmt.Println(k,v)
     }
     fmt.Println("-------------------------------4")
     //便利数组
     scoreMap["王五"] = 60
     delete(scoreMap,"李四") //删除对应的数据
     for k,v := range scoreMap{
     	fmt.Println(k,v)
     }
}