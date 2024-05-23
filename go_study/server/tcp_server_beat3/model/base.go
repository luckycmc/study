package model

import (
	"fmt"

	_ "github.com/go-sql-driver/mysql"
	"github.com/jmoiron/sqlx"
)

/**
  数据初始化的基类
**/
var Db *sqlx.DB
//Db  初始化
func InItDb() {
    database,err := sqlx.Open("mysql","root:123456@tcp(127.0.0.1)/blogweb_gin")
	if err !=nil {
		fmt.Println("open mysql failed",err)
	}
	Db = database
}
//redis的初始化
func InitRedis(){
	  
}