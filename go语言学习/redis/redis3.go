package main
import(
	"fmt"
	"github.com/garyburd/redigo/redis"
)


func main()  {
	  
	   //连接redis
	   c,err := redis.Dial("tcp","127.0.0.1:6379")
	   if err != nil{
		   fmt.Println("conn is err",err)
		   return
	   }
      defer c.Close()
	  //string 批量操作
	  _,err = c.Do("MSet","abc","100","efg",300)
	  if err != nil{
		fmt.Println(err)
		return
	}
	r,err := redis.Ints(c.Do("MGET","abc","efg"))
	if err != nil{
		fmt.Println("mget abc efg",err)
		return
	}
	for _,v := range r{
		fmt.Println(v)
	}

}