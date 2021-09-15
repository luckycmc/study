package main
import (
	"fmt"
	"os"
)
//定义图书结构体
type book struct{
	title    string
	author   string
	price    float32
	publish  bool
 }
//打印菜单信息
func showMenu(){
	fmt.Println("欢迎登陆BMS!")
    fmt.Println("1.添加书籍")
    fmt.Println("2.修改书籍")
    fmt.Println("3.展示所有书籍")
    fmt.Println("4.退出")
}
//输入书籍
func insertInput() *book{
	var (
		title    string 
		author   string
		price    float32
		publish  bool
	)
	fmt.Println("请根据提示输入相关内容")
    fmt.Print("请输入书名:")
    fmt.Scanln(&title)
    fmt.Print("请输入作者:")
    fmt.Scanln(&author)
    fmt.Print("请输入价格:")
    fmt.Scanln(&price)
    fmt.Print("请输入是否上架(true|false):")
	fmt.Scanln(&publish)
	fmt.Println(title,author,price,publish)
	//数据初始化
    book := newbook(title,author,price,publish)
    return book
}
//定义一个切片来实来存储,所有的书籍
var allbooks = make([]*book,0,200)
//定义一个结构体方法
func newbook(title,author string, price float32, publish bool) *book{
	return &book{
        title: title,
        author: author,
        price: price,
        publish: publish,
    }
}
//添加书籍函数
func addbook(){
	var (
		title string
        author string
        price float32
        publish bool
	)
	fmt.Println("请根据提示输入相关内容")
    fmt.Print("请输入书名:")
    fmt.Scanln(&title)
    fmt.Print("请输入作者:")
    fmt.Scanln(&author)
    fmt.Print("请输入价格:")
    fmt.Scanln(&price)
    fmt.Print("请输入是否上架(true|false):")
    fmt.Scanln(&publish)
    fmt.Println(title,author,price,publish)
	book := newbook(title,author,price,publish)
	//查看当前书籍是否存在
	for _,b := range allbooks{
		if b.title == book.title{
			fmt.Printf("<<%s这本书已经存在",book.title)
		}
		return
	}
	//把书籍添加到切片中
	allbooks = append(allbooks,book)
	fmt.Println("书籍添加成功!")
}
//修改书籍函数
func updatebook(){
	//获取要修改的信息
	book := insertInput()
	for index , b := range allbooks{
		//查找对应的信息是否存在
		if b.title == book.title{
			allbooks[index] = book
			fmt.Printf("书名:《%s》更新成功！",book.title)
            return 
		} 
		fmt.Printf("书名:《%s》不存在！", book.title )
	}
}
//5. 展示书籍的函数
func showbook(){
    if len(allbooks) == 0 {
        fmt.Println("啥也么有")
    }
    for _, b := range allbooks {
        fmt.Printf("《%s》作者：%s 价格:%.2f 是否上架销售: %t\n",b.title,b.author,b.price,b.publish)
    }
}
//主函数
func main(){
    for{
		//显示菜单
		showMenu()
		var option int
		//获取键盘输入的值
		fmt.Scanln(&option)
		switch option{
			case 1:
				addbook()
			case 2:
				updatebook()
			case 3:
				showbook()
			case 4:
				os.Exit(0)			
		}
	}
}