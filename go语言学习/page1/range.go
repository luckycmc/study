package main
import "fmt"

func main()  {
	 
	nums := []int{2,3,4}
	sum :=0
	for _,num := range nums{

		 sum += num
	}
	fmt.Println("sum:",sum)

	/*数组中的两个参数*/
	for i,num := range nums{
		if num ==3 {
			fmt.Println("index:",i)
		}
	}
	//map 
	kvs := map[string]string{"a":"apple","b":"banana"}
	for k,v := range kvs{
		fmt.Printf("%s -> %s\n", k, v)
	}
	//遍历字符串
	for k,v := range "go"{
		fmt.Println(k,v)
	}
}