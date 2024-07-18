package utils

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/go-sql-driver/mysql"
)

var DB *sql.DB

//初始化连接
func init(){
	db,err := sql.Open("mysql", "root:123456@tcp(127.0.0.1:3306)/blogweb_gin")
	//执行完释放连接
	//defer db.Close()

	if err != nil {
		 fmt.Println("mysql connect is error:",err)
		 return
	}
	DB = db
}

//操作数据库Sql
func ModifyDB(sql string,args ...interface{})(int64,error){
     
	   result,err := DB.Exec(sql,args...)
	   if err != nil {
		     log.Panicln(err)
			 return 0,err
	   }
	   //返回受影响的个数
	   count,err := result.RowsAffected()
	   if err != nil {
		   log.Panicln(err)
		   return 0,err
	   }
	   return count,err
}

//查询 相应数据 单个数据
func QueryRowDB(sql string) *sql.Row{
	
	  return DB.QueryRow(sql)
}