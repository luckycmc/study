package main

import(
	"fmt"
)

func d(n int) int{
    
	 if n== 0{  //递归终止条件
		 return 1
	 }

	 return n * d(n-1)
}
func main() {
	
	 fmt.Println(d(5))
}