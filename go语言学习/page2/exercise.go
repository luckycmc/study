package main
import (
	"fmt"
)
/**
  对数组进行排序
*/
func bubbleSort(arr []int) ([]int){
   for i := 0; i < len(arr)-1; i++ {
	for j := 0; j < len(arr)-1; j++ {
		if arr[j+1] < arr[j] {
			arr[j], arr[j+1] = arr[j+1], arr[j]
		}
	}
   }
   return arr
}
func main(){
	var a = make([]string, 5, 10)
	//var a = make([]int, 5, 10)
	fmt.Println(a)  //5个空格字符串
	fmt.Println(len(a))
	fmt.Printf("%p\n",a)
	for i := 0; i < 10; i++ {
		fmt.Println(fmt.Sprintf("%v", i))
		a = append(a, fmt.Sprintf("%v", i))
	}
	fmt.Println(a)//切片扩阵容了
	fmt.Println(len(a))
	fmt.Printf("%p\n",a)
	//对数组进行排序
	fmt.Println("********************************")
	var array1 =[]int{1,8,4,3,7,2}
	fmt.Println(bubbleSort(array1))
}