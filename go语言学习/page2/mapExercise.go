package main
import ( 
	"fmt"
	"strings"
)
func main(){
	str := "how do you do"
	//定义一个map
	worldCount := make(map[string]int,10)
	//获取切片的值
	worlds := strings.Split(str," ")
	//fmt.Println(worldCount)
	//遍历对应的切片
	for _,word := range worlds{
		   v,ok := worldCount[word]
		   if ok{
			   worldCount[word] = v+1 //累计加1
		   }else{
			worldCount[word] = 1//初始值为1
		   }
	}
	fmt.Println(worldCount)
}