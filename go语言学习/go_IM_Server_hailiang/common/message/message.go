package message

const(
	LoginMessageType = "LoginMes"
	LoginResMesType  = "LoginResMes"
	RegisterMesType  = "RegisterMes"
)


//消息结构体
type Message struct{
	Type string `json:"type"` //消息类型
	Data string `json:"data"` //消息数据
}

//登陆信息
type LoginMes struct{
	UserId int       `json:"userId"`//用户id
	UserPwd string   `json:"userPwd"`//用户密码
	UserName string  `json:"userName"`//用户名
}

//返回状态信息
type LoginResMes struct{
	Code int            `json:"code"`//返回500 表示该用户为注册  200 表示登陆成功
	Error string        `json:"error"`//返回错误信息
}

//注册的结构体
type RegisterMes struct {

}