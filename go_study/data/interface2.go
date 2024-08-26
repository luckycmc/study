package main

import "fmt"

// Product 是一个接口，定义了所有产品应该实现的方法。
type Product interface {
   Price() float64
}

// Book 结构体代表书籍，实现了 Product 接口。
type Book struct {
   Title  string
   Author string
   P  float64
}

func (b Book) Price() float64 {
   return b.P
}

// Computer 结构体代表电脑，也实现了 Product 接口。
type Computer struct {
   Brand  string
   P  float64
}

func (c Computer) Price() float64 {
   return c.P
}

// ProductFactory 是一个工厂，用于创建并返回实现了 Product 接口的对象。也就是实现接口的结构体
func ProductFactory(itemType string) Product {
   switch itemType {
   case "book":
      return Book{"Go语言编程", "Kimi", 120.0}
   case "computer":
      return Computer{"Dell", 6000.0}
   default:
      return nil // 或者可以返回一个错误
   }
}

func main() {
   // 获取不同类型的产品。
   book := ProductFactory("book")
   computer := ProductFactory("computer")

   // 通过接口调用 Price 方法。
   if book != nil {
      fmt.Printf("The price of the book is: %.2f\n", book.Price())
   }
   if computer != nil {
      fmt.Printf("The price of the computer is: %.2f\n", computer.Price())
   }
}