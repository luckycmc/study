package main

import "fmt"

/**
 golang  中的字符
**/
func main() {
	s1 := "hello"
	byte1 := []byte(s1)
	byte1[0] = 'H'
	fmt.Println(string(byte1))
}