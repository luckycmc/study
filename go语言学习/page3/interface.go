package main
import "fmt"
//定义一个移动接口
type Mover interface{
	move()
}
type dog struct{
	name string
}
type car struct{
	brand string
}
//dog 类实现Mover接口
func(d dog) move(){
   fmt.Printf("%s会跑的\n",d.name)
}
//car 类实现的mover接口
func(c car) move(){
	fmt.Printf("%s速度70迈\n", c.brand)
}
func main(){
	var x Mover  //默认是个nil
	//fmt.Printf("%T",x)
	var a = dog{
		name:"旺财",
	}
	var b = car{
		brand:"保时捷",
	}

	x =a 
	x.move()
	x = b
	x.move()
	fmt.Printf("%T\n",x)
}