package common

const(
	LoginMessageType = "LoginMes"
	LoginResMesType  = "LoginResMes"
)


//消息结构体
type Message struct{
	Type string //消息类型
	Data string //消息数据
}

//登陆信息
type LoginMes struct{
	UserId int //用户id
	UserPwd string //用户密码
	UserName string //用户名
}

//返回状态信息
type LoginResMes struct{
	Code int            //返回500 表示该用户为注册  200 表示登陆成功
	Error string        //返回错误信息
}
