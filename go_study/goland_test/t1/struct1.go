package main
import(
	"fmt"
)
/*
  golang的方法总是绑定对象的实例 并且隐藏实例 作为第一个 实参
*/
// 相当于 php的类 class  里面的值就是属性
type User struct {

	  Name string
	  Age  int
}
//和User类进行绑定 接受者是一个 User 类型的值
//要调用 Notify 方法我们需要一个 User 类型的值或者指针
func(u User) Notify(){
   
	  fmt.Printf("%v : %v\n",u.Name,u.Age)
}

func main(){
    //值类型调用该方法
	u1 := User{"zpw",32}  //相当于 php的 $obj = new User
	u1.Notify()
	//指针类型调用该方法
	u2 := User{"sunfeng",29}
	u3 := &u2
	u3.Notify()
}
