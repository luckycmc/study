package main

//书籍结构体
type Book struct{
	ID int64 `db:"id"`
	Title string `db:"title"`
	Price float64 `db:"price"`
	Publisher_id int64 `db:"publisher_id"`
	Publisher
}

//发布者结构体
type Publisher struct{
	ID int64 `db:"publisher_id"`
	Province string `db:"province"`
	City string `db:"city"`
}