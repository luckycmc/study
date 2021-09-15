package main
import "fmt"
/*使用“面向对象”的思维方式编写一个学生信息管理系统。
学生有id、姓名、年龄、分数等信息
程序提供展示学生列表、添加学生、编辑学生信息、删除学生等功能*/
type Student struct{
	id      int
	age     int
	score   int
	name    string
}
//定义一个map 地图
var StudentDta = make([]map[string][string])
//添加操作
func(s *Student) add(){

}
//修改操作
func(s *Student) edit(){
	
}
//删除操作
func(s *Student) delete(){
	
}
//展示操作
func(s *Student) showList(){
	
}
//主函数
func main(){
    
}