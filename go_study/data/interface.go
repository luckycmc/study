package main

import "fmt"

// Animal 是一个接口，定义了所有动物应该实现的方法。
type Animal interface {
	MakeSound()
}

// Dog 实现了 Animal 接口。
type Dog struct{}

func (d Dog) MakeSound() {
	fmt.Println("Woof!")
}

// Cat 实现了 Animal 接口。
type Cat struct{}

func (c Cat) MakeSound() {
	fmt.Println("Meow!")
}

// GetAnimal 返回一个实现了 Animal 接口的实例。 也就是说只要实现了他的接口 实例化后就是他的对象
func GetAnimal(animalType string) Animal {
	switch animalType {
	case "dog":
		return Dog{}
	case "cat":
		return Cat{}
	default:
		return nil // 或者可以返回一个错误
	}
}

func main() {
	// 根据输入获取不同的动物实例。
	animal := GetAnimal("dog")
	if animal != nil {
		animal.MakeSound()
	}
}