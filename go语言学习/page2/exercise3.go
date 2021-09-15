package main
import (
	"fmt"
	"strings"
)
/*
你有50枚金币，需要分配给以下几个人：Matthew,Sarah,Augustus,Heidi,Emilie,Peter,Giana,Adriano,Aaron,Elizabeth。
分配规则如下：
a. 名字中每包含1个'e'或'E'分1枚金币
b. 名字中每包含1个'i'或'I'分2枚金币
c. 名字中每包含1个'o'或'O'分3枚金币
d: 名字中每包含1个'u'或'U'分4枚金币
写一个程序，计算每个用户分到多少金币，以及最后剩余多少金币？
程序结构如下，请实现 ‘dispatchCoin’ 函数
*/
const(
	coins = 50  //总金币个数
)
//分发函数
func dispatchCoin() (int,map[string]int){

	users := []string{
		"Matthew", "Sarah", "Augustus", "Heidi", "Emilie", "Peter", "Giana", "Adriano", "Aaron", "Elizabeth",
	}
	distribution := make(map[string]int, len(users)) //分发用户对应的金币

	left := coins
	//遍历用户查找对应的数据
     for _,user := range users{
         if strings.Contains(user,"e") || strings.Contains(user,"e"){
			distribution[user]= distribution[user]+1
			left = left -1
		 }
		 if strings.Contains(user,"i") || strings.Contains(user,"I"){
			distribution[user]= distribution[user]+2
			left = left -2
		 }
		 if strings.Contains(user,"o") || strings.Contains(user,"O"){
			distribution[user]= distribution[user]+3
			left = left -3
		 }
		 if strings.Contains(user,"u") || strings.Contains(user,"U"){
			distribution[user]= distribution[user]+5
			left = left -5
		 }
	 }
	 return left,distribution
}
//主函数
func main(){
	left,mapStudents:= dispatchCoin()
	fmt.Println("剩下：", left)
	sum := 0
	for name,score := range mapStudents{
		fmt.Println(name,"有几个",score,"金币")
        sum+=score;
	}
	fmt.Println(sum)
}