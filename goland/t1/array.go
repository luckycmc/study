package main

import "fmt"

func printArray(arr *[5]int){
    arr[0] = 10
	for i,v := range arr {
		fmt.Println(i,v)
	}
}

func main() {
    var arr1 [5]int
	printArray(&arr1)
	fmt.Println(arr1);
	//数组2
	arr2 := [...]int{1,2,3,4,5}
	printArray(&arr2)
    fmt.Println(arr2)
}