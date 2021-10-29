package main
import (
	"fmt"
	"time"
)
//tcp 流量控制算法 慢启动  拥塞 避免 快回复  快重传 拥塞窗口
/****
  指数增长
*/
func zhishu(size int) int {
	  
	   if size == 0 {
		    return 1
	   }
	   return size*2  //二倍的速度增加
}
/****
  加法增长
**/
func jiafa(size int) int {
	  
	return size+1  //每一次加1
}
func main() {
     
	 iniSzie := 0     //初始值
     ssthresh := 64    //门限值
     yongsai  := 80   //用塞值
	 for {
          
		   if iniSzie < ssthresh {

			   fmt.Println("-----------------慢开始算法————————————————————————")
			   iniSzie = zhishu(iniSzie)
			   fmt.Println("指数增长.....",iniSzie)
		   }else if iniSzie <yongsai{

			    fmt.Println("-----------------避免拥塞算法—————————————————————-")
				iniSzie = jiafa(iniSzie)
				fmt.Println("加法增长.....",iniSzie)
				time.Sleep(time.Second);
		   }else{
			    fmt.Println("-----------------快恢复算法————————————————————————")
			    ssthresh = iniSzie/2
                iniSzie  = iniSzie/2
		   }  
		  
	 }

}