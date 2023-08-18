package main

import(
	"fmt"
	_ "github.com/go-sql-driver/mysql"
	"github.com/jmoiron/sqlx"
)

var db *sqlx.DB
//初始化数据库
func initDB()(err error){
    
    database, err := sqlx.Open("mysql", "root:123456@tcp(127.0.0.1:3306)/go_test")
    if err != nil {
        fmt.Println("open mysql failed,", err)
        return
    }
    db = database
    //设置连接池的大小
    db.SetMaxOpenConns(100)
	db.SetMaxIdleConns(60)
	return
}

// 插入数据
func insertBook(title string, price float64)(err error){
	sqlStr := "insert into book(title,price) values(?,?)"
	r,err := db.Exec(sqlStr, title, price)
	if err != nil {
        fmt.Println("exec failed, ", err)
        return
    }
    id, err := r.LastInsertId()
    if err != nil {
        fmt.Println("exec failed, ", err)
        return
    }
    fmt.Println("insert succ:", id)
    return
}
//获取用户列表数据
func queryAllBook()(bookList []*Book,err error){
    sqlStr := "select id,title,price from book"
	err = db.Select(&bookList, sqlStr)
	if err != nil {
		fmt.Println("查询所有书籍信息失败！",err)
		return
	}
	return 
}
