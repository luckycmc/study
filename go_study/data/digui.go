package main

import "fmt"

func Digui(n int) int {
	//必须有结束的条件
	if n == 0 {
		return 1
	}
	return n * Digui(n-1)
}
func main() {
	fmt.Println(Digui(5))
}
