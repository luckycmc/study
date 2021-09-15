package main
import "fmt"

func main(){
	/*var i int=1
	i++
	++i //golang语言没有 ++i这种类型的自增
	fmt.Println("i自增后的值",i)*/
	var score int
	fmt.Println("请输入你的成绩:")
	fmt.Scanf("%d",&score)
	//fmt.Println(score)
	/*switch key{
		case 'a':
			fmt.Println("aa");
		case 'b':
		    fmt.Println("bb");
		case 'c':
		    fmt.Println("星星好好学习。。。");
		default:
		    fmt.Println("你输入有误....")        	
	}*/
	switch{
		case score == 60 :
			fmt.Println("你的成绩在及格线,加油哦")
		case score == 70 :
			fmt.Println("你的成绩是：",score)	
		default :
		    fmt.Println("非法输入....")	
	}
}
	


