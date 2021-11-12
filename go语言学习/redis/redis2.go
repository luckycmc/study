package main
import (
	"fmt"
	"github.com/garyburd/redigo/redis"
)
func main(){
	  //连接redis
	  c,err := redis.Dial("tcp","localhost:6379")
	  if err != nil{
		  fmt.Println("redis conn is err:=",err)
		  return
	  }
      
	  defer c.Close()

	  //设置 对应的消息
	  _,err := c.Do("set","abc","100")
	  if err != nil{
		  fmt.Println(err)
		  return
	  }
	  //获取对应的消息
	  r,err := redis.Int(c.Do("get","abc"))  //强制转换成整型
	  if err != nil{
		fmt.Println(err)
		return
	}
	fmt.Println(r)
	fmt.Println("redis success")
}