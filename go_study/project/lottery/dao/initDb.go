package dao

import (
	"fmt"
	"lottery/conf"

	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

var DB gorm.DB
//初始化db 方法
func init(){
       // "mysql","root:Qazwsx123@[tcp](localhost:3306)/test_gorm?charset=utf8&parseTime=True&loc=Local"
	   dbInfo := fmt.Sprintf("%s:%s@[tcp](%s:%d)/%s?charset=utf8&parseTime=True&loc=Local",
	   conf.DbMaster.User,conf.DbMaster.Pwd,conf.DbMaster.Host,conf.DbMaster.Port,conf.DbMaster.Database)
	   fmt.Println(dbInfo)
	   db,err := gorm.Open(mysql.Open(dbInfo),&gorm.Config{})
	   if err != nil {
		  panic(err)
	   }
       DB = *db
}