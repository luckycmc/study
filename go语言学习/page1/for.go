package main
import  "fmt"

func main() {
	var str string = "Hellow world"
	fmt.Printf("这是for循环遍历。。。。\n")
	for i := 0; i < len(str); i++ {
		 fmt.Printf("%c\n",str[i])
	}
	fmt.Printf("这是for-range循环遍历。。。。\n")
	for index,val:=range str{
		fmt.Printf("index=%d,val=%c\n",index,val)
	}
	fmt.Printf("这是for循环遍历中文。。。。\n")
	str="德亨文"
	for index,val:=range str{
		fmt.Printf("index=%d,val=%c\n",index,val)
	}
}