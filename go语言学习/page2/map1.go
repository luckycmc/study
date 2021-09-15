package main
import "fmt"
func main(){
	scoreMap:=make(map[string]int,8)
	scoreMap["小明"]=90
	scoreMap["张三"]=100
	fmt.Println(scoreMap) //打印出地图中的数据
	//通过键值找对应的值
	fmt.Println(scoreMap["小明"])
	//查看map的数据类型
	fmt.Printf("%T\n",scoreMap)
	//直接初始化赋值
	userInfo := map[string]string{
		"username": "沙河小王子",
		"password": "123456",
	}
	fmt.Println(userInfo) 
	v,ok:=scoreMap["小明"]
	if ok{
        fmt.Println(v)
	}else{
		fmt.Println("无此人")
	}
	//遍历userinfo map 错误，提示 	:=要和等号连载一起
	for k,v := range userInfo {
		fmt.Println(k,v)
	}
	//自遍历k值
	for k := range scoreMap{
		fmt.Println(k)
	}
	//map 是通过delete函数删除的 delete(map 名称，键值)
	//delete(scoreMap,"张三")
	fmt.Println("遍历删除后的数据")
	for k,v := range scoreMap{
		//fmt.Println(k,v)
		fmt.Printf("%p\n",k)
		fmt.Printf("%p\n",v)
	}
	fmt.Printf("%p\n",scoreMap)
	fmt.Printf("%v\n",scoreMap["小明"])
}
