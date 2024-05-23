package model

import "fmt"

/**
  主要实现用户的功能
**/
type User struct {
	UserName string `db:"username"`
	PassWord string `db:"password"`
}

// 用户的注册
func SaveUser(data *User) bool {
	sql := "insert into person(username, password)values(?, ?)"
	r, err := Db.Exec(sql, data.UserName, data.PassWord)
	if err != nil {
		fmt.Println("exec failed,",err)
		return false
	}
	_,err = r.LastInsertId()
	if err !=nil {
		fmt.Println("exec failed, ", err)
        return false
	}
	return true
}

func Login(data *User) bool{
     sql := "select username,password from person where username =? and password=?"
	 var stu User
	 Db.Get(&stu,sql,data.UserName,data.PassWord)
	 if stu.PassWord == "" {
		 return false
	 }
	 fmt.Println("select succ:",stu)
	 return true
}