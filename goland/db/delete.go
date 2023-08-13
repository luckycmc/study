package main 

import(
	"fmt"
	 _ "github.com/go-sql-driver/mysql"
    "github.com/jmoiron/sqlx"
)

var Db *sqlx.DB //全局Db对象

func init(){
	//连接mysql获取mysql 对象
	database,err := sqlx.Open("mysql", "root:root@tcp(127.0.0.1:3306)/test")
	if err != nil {
		fmt.Println("connect mysql is err:",err)
		return 
	}
	Db = database
}

func main(){
	res,err := Db.Exec("delete from person where user_id=?",2)
	if err != nil {
		fmt.Println("delete is err:",err)
		return 
	}
	//获取影响的行数
	row,err:= res.RowsAffected()
	if err != nil {
		fmt.Println("rows failed, ",err)
	}
	 fmt.Println("delete succ: ",row)
}