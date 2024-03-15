package main

import "fmt"

func printArray(arr *[5]int) {
	arr[0] = 10
	for i, v := range arr {
		fmt.Println(i, v)
	}
}

func main() {
	var arr1 [5]int   //定义一个 数组 默认都是 {0,0,0,0}
	printArray(&arr1) //指针传参打印数据
	fmt.Println(arr1)
	//数组2
	arr2 := [...]int{1, 2, 3, 4, 5} //定义数组2 赋值
	printArray(&arr2)
	fmt.Printtln(arr2)
}
