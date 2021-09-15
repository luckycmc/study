package main
import "fmt"
func main(){
	//在go语言中数组的长度必须是常量,并切长度是数组类型的一部分，一单定影长度不能改变
	var a [3]int
	a=[3]int{1,2,3}
	fmt.Printf("%T\n",a)//数组类型
	fmt.Println(a)   //数组中的数据[1,2,3]
	/*var b [4]int
	b=a //不同类型的数据
	fmt.Println(b)//cannot use a (type [3]int) as type [4]int in assignment*/
	var numArray = [...]int{1, 2} //自行推导数组的长度
	var cityArray = [...]string{"北京", "上海", "深圳"}
	fmt.Printf("%T\n",numArray)//[2] int
	fmt.Println(numArray)      //[1,2]
	fmt.Printf("%T\n",cityArray)//[3][string]
	fmt.Println(cityArray)     //[北京 上海 深圳]
	fmt.Println("**************************************************************")
	//指定索引初始化数组
	a1:=[...]int{1:1,3:5}   //索引值为1的对应的值为1,索引值为3的对应的为5
	fmt.Println(a1)                  // [0 1 0 5]
	fmt.Printf("type of a:%T\n", a1) //type of a:[4]int
	var a2 = [...]string{"北京", "上海", "深圳"}
	// 方法1：for循环遍历
	for i := 0; i < len(a); i++ {
		fmt.Println(a2[i])
	}

	// 方法2：for range遍历
	for index, value := range a2 {
		fmt.Println(index, value)
	}
	//go语言二维数组的定义
	a3:=[3][2]string{
		{"北京","上海"},
		{"广州","深圳"},
		{"成都","重庆"},
	}
	fmt.Printf("%T\n",a3)//[3][2]string 二维数组三行两列
	fmt.Println(a3)  //[[北京 上海] [广州 深圳] [成都 重庆]]
	//更具索引取值
	fmt.Println(a3[2][1]) //对应的重庆
	//对二维数组进行遍历
	for k1,v1:=range a3{
		for k2,v2:=range v1{
			fmt.Printf("%d %s\t",k2,v2)
		}
		fmt.Println(k1)
	}
	//求和
	scores:=[4]int{20,78,89,100}
	var sum int
	//遍历相加即可
	for _,score := range scores{
		sum+=score
	}
	fmt.Println("学生的成绩综合为:",sum)
	//找出数组中和为指定值的两个元素的下标，
	//比如从数组[1, 3, 5, 7, 8]中找出和为8的两个元素的下标分别为(0,3)和(1,2)
	var numbers [5]int = [5]int{1, 3, 5, 7, 8}
	numbersLen:= len(numbers)
	//fmt.Println(numbersLen)
	for i:=0;i<numbersLen;i++{
		for j:=1;j<numbersLen;j++{
			//找出两个值为8的下标
			if numbers[i]+numbers[j] == 8{
				fmt.Println("(",i,j,")")
			}
		}
	}
}