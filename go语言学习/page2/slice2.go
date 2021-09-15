package main
import "fmt"
func main(){
	//append 添加元素和切片扩容
	/*
	  Go语言的内建函数append()可以为切片动态添加元素。 
	  每个切片会指向一个底层数组，这个数组能容纳一定数量的元素。
	  当底层数组不能容纳新增的元素时，切片就会自动按照一定的策略进行“扩容”，此时该切片指向的底层数组就会更换。
	  “扩容”操作往往发生在append()函数调用时。
	  @1.append()函数将元素追加到切片的最后并返回该切片。
	  @2.每一次扩容都是扩容前的两倍
	*/
	var numSlice []int
	for i := 0; i < 10; i++ {
		numSlice = append(numSlice, i)//操出底层数组,时候底层数组会改变                
		fmt.Printf("%v  len:%d  cap:%d  ptr:%p\n", numSlice, len(numSlice), cap(numSlice), numSlice)
	}
	//@append 支持一次追加多个元素
	var citySlice []string  //只是定义并没有初始化分配内存
	if citySlice == nil {
		//切片没有初始化默认为nil 因为没有分配内存
		fmt.Println("citySlice is nil")
	}
	fmt.Printf("%p\n",citySlice)
	citySlice = append(citySlice,"北京")
	fmt.Printf("%p\n",citySlice)
	//追加多个元素
	citySlice = append(citySlice,"上海","天津")
	fmt.Printf("%v  %p\n",citySlice,citySlice)
	//追加整个切片
	a := []string{"成都", "重庆"}
    citySlice = append(citySlice, a...)
	fmt.Printf("%v  %p\n",citySlice,citySlice)
	fmt.Println("从切片中删除元素")
	// 从切片中删除元素
	a1 := []int{30, 31, 32, 33, 34, 35, 36, 37}
	// 要删除索引为2的元素
	a1 = append(a1[:2], a1[3:]...)
	//要从切片a中删除索引为index的元素，操作方法是a = append(a[:index], a[index+1:]...)
	fmt.Println(a1) 
}