package main
import(
	"fmt"
	"errors"
)
//使用一个结构体管理队列
type Queue struct{
	maxSize int
	array [5]int   //数组=>模拟队列
	front int      //表示指向队首
	rear  int      // 表示指向队列尾部
}
//添加数据到队列
func (this *Queue) AddQueue(val int)(err error) {
     
	   //首先判断队列是否已满
       if this.rear == this.maxSize -1 {
		   return errors.New("queue full")
	   }
	   //尾部节点后移
	   this.rear++
	   this.array[this.rear] = val
	   return
}
//显示队列 找到对首 遍历到对尾
func(this *Queue) ShowQueue(){
      fmt.Println("队列当前的情况是:")
	  //this.front 不包含队首
	  for i:= this.front+1;i<= this.rear;i++{
            fmt.Printf("array[%d]=%d\t",i,this.array[i])
	  }
}
//从队列中取出数据
func(this *Queue) GetQueue() (val int,err error) {
       
	    //首先判断队列是否为空
		if this.rear == this.front{
			return -1,errors.New("queue empty")
		}
		//队首节点后移
		this.front++
		val = this.array[this.front]
        return val,err
}
//主函数
func main()  {
	  
	  //创建一个队列 并且初始化
	  queue := &Queue{
		  maxSize:5,
		  front:-1,
		  rear:-1,
	  }
      //添加数据
	  queue.AddQueue(1) 
	  queue.AddQueue(2) 
	  queue.AddQueue(3) 
	  queue.AddQueue(4) 
	  queue.AddQueue(5) 
	  //显示队列
	  queue.ShowQueue()
	  queue.GetQueue()

	  queue.ShowQueue()
}