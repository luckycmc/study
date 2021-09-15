package main
import "fmt"
//遍历切片的每一个内容并通过指定函数访问
func visit(list []int,f func(int)){
	for _,v :=range list{
		f(v)  //f() 调用的救治指定的匿名和函数
	}
}
func main(){
	//使用匿名函数打印切片内容
	visit([]int{1,2,3,4},func(v int){
		fmt.Println(v)
	})
}