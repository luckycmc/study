package models

type User struct {
	Id       int
	Username string
	Age      int
	Sex      int
}

// 返回对应的表名
func (User) TableName() string {
	return "users"
}
