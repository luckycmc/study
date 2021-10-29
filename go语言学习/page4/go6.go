package main
import (
	"fmt"
	"time"
)
const N = 1000000
func main()  {
	
	  fmt.Println("run in main goroute")
	  i := 1
	  for{
		  go func ()  {
			  for{
				  time.Sleep(time.Second)
			  } 
		  }()

          if i%10000 ==0 {
			  fmt.Printf("%d grontinue	stared\n",i)
		  }
		  i++
		  if i==N{
			  break
		  }

	  }
	  time.Sleep(time.Second * 10)
}