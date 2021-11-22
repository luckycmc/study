package main
import(
	"fmt"
)

type ValNode struct{
	row int
	col int
	val int
}

func main()  {
	
	  //先创建一个原始数组
	  var chessMap [11][11]int
	  chessMap[1][2] = 1 //黑子
	  chessMap[2][3] = 2 //篮子

	  //输出原始数组
	  for _,v := range chessMap{
		for _,v1 := range v{
		    fmt.Printf("%d\t",v1)
		}
		fmt.Println()
	  }
	  //转换成稀疏数组
	  //1.为0的元素不存放  创建一个 node 结构体
	  //将其放入切片中
	  var sparselArr []ValNode 
      
	  //标准的的一个稀疏数组应该还有一个,记录二位数组的元素(行和列默认值)
	  //创建一个初始节点
	  valNode := ValNode{
		  row:11,
		  col:11,
		  val:0,
	  }
	  //加入初始节点
      sparselArr = append(sparselArr,valNode)

	  for i,v := range chessMap{
		for j,v2 := range v{
			  
			   if v2 !=0 {
				   
				    //创建一个node 节点
					valNode := ValNode{
						row:i,
						col:j,
						val:v2,
					}
					sparselArr = append(sparselArr,valNode)
			   }
		}
		
	  }
	  fmt.Println(sparselArr)
}