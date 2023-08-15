package main

import (
	"fmt"
    _ "github.com/go-sql-driver/mysql"
    "github.com/jmoiron/sqlx"
)

//声明全局对象
var Db *sqlx.DB

func init(){
    database, err := sqlx.Open("mysql", "root:root@tcp(127.0.0.1:3306)/test")
    if err != nil {
        fmt.Println("open mysql failed,", err)
        return
    }
    Db = database
}

func main(){
	//43 修改用户名
	server := NewServer("0.0.0.0",8888)
	server.Start()
}