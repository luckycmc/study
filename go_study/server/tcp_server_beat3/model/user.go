package model

import "fmt"

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