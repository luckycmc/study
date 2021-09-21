package main

import "fmt"

func main()  {
	
	 var n[10] int   //C语言定义 int a[10];
     
	 var i,j int
     
	 //为n个数组初始化
	 for i = 0; i < 10; i++ {
		 n[i] = i+100
	 }
	 //循环输出数组元素
	 for j = 0; j< 10; j++ {

		  fmt.Printf("Element[%d] = %d\n", j,n[j])
	 }

	 //用... 自定义数组长度  自动推导类型
	 blance :=[5]float32{2.0,8.0,9.0,50.0} //自动推导类型是固定的
     blance[4] =99.00
	 for k,v :=range blance{

		fmt.Printf("%d------>%f\n",k, v)
	 }

}