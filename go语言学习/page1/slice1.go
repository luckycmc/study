package main

import "fmt"

func main()  {
	 
	 var numbers []int
     printSlice(numbers);

	 //切片追加
	 numbers = append(numbers,0)
	 printSlice(numbers)

	 numbers = append(numbers,1)
	 printSlice(numbers)
    
	 //添加多个元素
	 numbers = append(numbers,2,3,4)
	 printSlice(numbers)
	 //用make创建多个切片numbers1 是直接切片的两倍

	 numbers1:=make([]int, len(numbers),(cap(numbers))*2)
	 /*将number中的数据cpoy numbers1中*/
	 copy(numbers1,numbers)
	 printSlice(numbers1)
}

func printSlice(x []int)  {
	  
	  fmt.Printf("len=%d cap=%d slice=%v\n",len(x),cap(x),x);
}