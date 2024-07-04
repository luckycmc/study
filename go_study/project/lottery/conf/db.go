package conf

const DriverName = "mysql"

type DbConfig struct {
	Host     string
	Port     int
	User     string
	Pwd      string
	Database string
}

// 定义一个切片 后期可能有很多个数据库
var DbMasterList = []DbConfig{
	{
		Host:     "127.0.0.1",
		Port:     3306,
		User:     "root",
		Pwd:      "root",
		Database: "lottery",
	},
}

var DbMaster DbConfig = DbMasterList[0]