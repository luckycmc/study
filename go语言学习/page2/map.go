package main
import "fmt"
func main(){
   var scoreMap map[string]int //map是引用传值没有初始化则则为nil
   fmt.Printf("%v %p\n",scoreMap,scoreMap)
   if scoreMap==nil{
	   fmt.Println("ok")
   }
   //通过make进行初始化
   map1:=make(map[string]int,8)
   map1["tom"] = 88
   //map1["tom"] = 99 //同名键值对会产生覆盖
   map1["tom1"] = 81
   map1["tom2"] = 81
   map1["tom3"] = 81
   map1["tom4"] = 81
   map1["tom5"] = 81
   map1["tom6"] = 81
   map1["tom7"] = 81
   fmt.Printf("%v  %p\n",map1,map1)
   map1["tom8"] = 89
   map1["tom9"] = 89
   map1["tom10"] = 89
   fmt.Println(map1,len(map1))
   fmt.Printf("%v %p\n",map1,map1)
   fmt.Println(cap(map1["tom"]))
}