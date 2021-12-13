package main

import (
	"fmt"
)

func main()  {
	 
	  list:= NewArrayList()
	  list.Append(1)
	  list.Append(2)
	  list.Append(3)
	  fmt.Println(list)
	  fmt.Println(list.TheSize)
}