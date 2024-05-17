package main

import (
    "fmt"

    _ "github.com/go-sql-driver/mysql"
    "github.com/jmoiron/sqlx"
)

type Person struct {
    UserId   int    `db:"user_id"`
    Username string `db:"username"`
    Sex      string `db:"sex"`
    Email    string `db:"email"`
}

type Place struct {
    Country string `db:"country"`
    City    string `db:"city"`
    TelCode int    `db:"telcode"`
}

var Db *sqlx.DB
//初始化mysql
func init(){
    
     database,err := sqlx.Open("mysql","root:root@tcp(127.0.0.1:3306)/test")
     if err != nil {
         fmt.Println("mysql connect is err:",err)
         return 
     }
     Db = database
}
//主函数
func main(){
    res,err := Db.Exec("update person set username=? where user_id=?", "stu0003", 2)
    if err != nil {
        fmt.Println("exec failed, ", err)
        return
    }
    row,err := res.RowsAffected()
    if err != nil {
        fmt.Println("rows failed, ",err)
        return 
    }
    fmt.Println("update succ:",row)
}