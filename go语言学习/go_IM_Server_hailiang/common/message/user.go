package message

//定义一个用户结构体
type User struct{
	//为了保证序列化和反序列化成功 必须保证tag保持一致
	UserId int         `json:"userId"`
	UserPwd string     `json:"userPwd"`
	UserName string    `json:"userName"`
}
