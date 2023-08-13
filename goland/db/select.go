package main

import (
    "fmt"

    _ "github.com/go-sql-driver/mysql"
    "github.com/jmoiron/sqlx"
)
//person
type Person struct{
    UserId    int   `db:"user_id"`
    Username string `db:"username"`
    Sex      string `db:"sex"`
    Email    string `db:"email"`
}

//Place 
type Place struct{
    Country string `db:"country"`
    City    string `db:"city"`
    TelCode int    `db:"telcode"`
}
//声明全局变量
var Db *sqlx.DB

// init 初始化函数
func init(){
    database,err := sqlx.Open("mysql","root:root@tcp(127.0.0.1:3306)/test")
    if err != nil {
        fmt.Println("connect mysql is err:",err)
        return 
    }
    //赋值给全局变量
    Db = database
}

func main() {

    var person []Person
    //person 被赋值
    err := Db.Select(&person, "select user_id, username, sex, email from person limit 10")
    if err != nil {
        fmt.Println("exec failed, ", err)
        return
    }

    for k,v :=range person{
        fmt.Println(k,v)
    }
}