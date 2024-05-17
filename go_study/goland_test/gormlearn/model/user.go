package model

import (
	"log"
)
//对应的数据结构
type User struct {
	ID int64 // 主键
	//通过在字段后面的标签说明，定义golang字段和表字段的关系
	//例如 `gorm:"column:username"` 标签说明含义是: Mysql表的列名（字段名)为username
	Username string `gorm:"column:username"`
	Password string `gorm:"column:password"`
	//创建时间，时间戳
	CreateTime int64 `gorm:"column:createtime"`
}

// 设置表名
func (u User) TableName() string {
	return "users"
}
//保存用户信息
func SaveUser(user *User) {
	// 数据库的操作，连接数据库
	err := DB.Create(user).Error
	if err != nil {
		log.Println("insert user error：", err)
	}
}
//获取单个数据
func GetById(id int64) User{
	 
	 var user User
	 err := DB.Where("id=?",id).First(&user)
	 if err != nil{
		log.Println("get user info is error")
	 }
	 return user
}

//获取所有数据
func GetAll() []User{
     var users []User
	 err := DB.Find(&users).Error
	 if err != nil{
		log.Println("get user all error:", err)
	 }
	 return users
}
