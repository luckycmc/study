package model

import(
    _"fmt"
	"../../common/message"
	"net"
)
//因为在客户端很多地方会用到curUser 因此我们要将其作为全局的
type CurUser struct{

	  Conn net.Conn
	  message.User
}

