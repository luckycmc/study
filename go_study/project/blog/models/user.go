package models

import (
	"blog/utils"
	"fmt"
)

type User struct {
	Id         int
	Username   string
	Password   string
	Status     int //0 正常状态 ，1 删除
	Createtime int64
}
/*****************数据库的操作 start*********************/
//插入数据
func InsertUser(user User) (int64, error) {
	sql:= "insert into users(username,password,status,createtime) values (?,?,?,?)"
	return utils.ModifyDB(sql,user.Username,user.Password,user.Status,user.Createtime)
}
//按条件查询 相应的数据
func QueryUserWightCon(con string) int {
    sql := fmt.Sprintf("select id from users %s", con)
    fmt.Println(sql)
    row := utils.QueryRowDB(sql)
    id := 0
    row.Scan(&id)
    return id
}

//根据用户名查询id
func QueryUserWithUsername(username string) int {
    sql := fmt.Sprintf("where username='%s'", username)
    return QueryUserWightCon(sql)
}


//根据用户名和密码，查询id
func QueryUserWithParam(username ,password string)int{
    sql:=fmt.Sprintf("where username='%s' and password='%s'",username,password)
    return QueryUserWightCon(sql)
}

/*****************数据库的操作 end*********************/

