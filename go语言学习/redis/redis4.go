package main

import(
	"fmt"
	"github.com/garyburd/redigo/redis"
)

func main(){
	c,err := redis.Dial("tcp","127.0.0.1:6379")
	if err != nil{
		fmt.Println("conn is err :",err)
		return
	}
	defer c.Close()
	_,err = c.Do("lpush","book_list","abc","300")
	if err != nil{
		fmt.Println(err)
		return
	}
	//获取其中的数据 获取对列中的数据
	values,err := redis.Values(c.Do("lrange","book_list","0","-1"))
	if err != nil{
		fmt.Println("get abc failed,", err)
        return
	}
	for _,v := range values{
		fmt.Println(string(v.([]byte)))
	}
}