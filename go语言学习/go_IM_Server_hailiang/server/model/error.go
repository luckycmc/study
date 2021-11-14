package model

import(
	"errors"
)
//更具业务逻辑的需要制定一些错误
var(
    ERROR_USER_NOTEXISTS = errors.New("用户不存在....")
	ERROR_USER_EXISTS    = errors.New("用户已经存在....")
	ERROR_USER_PWD       = errors.New("密码错误.....")
)