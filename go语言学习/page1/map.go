package main
import "fmt"

func main()  {
	
	 var countryCapitalMap map[string]string //创建集合
	 countryCapitalMap = make(map[string]string)   //相当于C 中malloc 申请内存
	 /* map插入key - value对,各个国家对应的首都 */
	 countryCapitalMap [ "France" ] = "巴黎"
	 countryCapitalMap [ "Italy" ] = "罗马"
	 countryCapitalMap [ "Japan" ] = "东京"
	 countryCapitalMap [ "India " ] = "新德里"

	 //遍历打印数据
	 for country := range countryCapitalMap {  //打印的是k
		fmt.Println(country, "首都是", countryCapitalMap [country])
	 }

	 capital, ok := countryCapitalMap [ "American" ] /*如果确定是真实的,则存在,否则不存在 */
	 if (ok) {
		fmt.Println("American 的首都是", capital)
	 }else{
		fmt.Println("American 的首都不存在")
	 }

}