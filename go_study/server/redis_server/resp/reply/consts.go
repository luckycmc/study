package reply

/**
 @常用信息回复
 ESP 定义了5种格式：

简单字符串(Simple String): 服务器用来返回简单的结果，比如"OK"。非二进制安全，且不允许换行。
错误信息(Error): 服务器用来返回简单的错误信息，比如"ERR Invalid Synatx"。非二进制安全，且不允许换行。
整数(Integer): llen、scard 等命令的返回值, 64位有符号整数
字符串(Bulk String): 二进制安全字符串, 比如 get 等命令的返回值
数组(Array, 又称 Multi Bulk Strings): Bulk String 数组，客户端发送指令以及 lrange 等命令响应的格式

RESP 通过第一个字符来表示格式：

简单字符串：以"+" 开始， 如："+OK\r\n"
错误：以"-" 开始，如："-ERR Invalid Synatx\r\n"
整数：以":"开始，如：":1\r\n"
字符串：以 $ 开始
数组：以 * 开始
**/
// PongReply is +PONG
type PongReply struct{}

var pongBytes = []byte("+PONG\r\n")

// ToBytes marshal redis.Reply
func (r *PongReply) ToBytes() []byte {
	return pongBytes
}

// ping  pong  reply
func MakePongReply() *PongReply {
	return &PongReply{}
}

/**************OK replay start********************/
// OkReply is +OK
type OkReply struct{}

var okBytes = []byte("+OK\r\n")

// ToBytes marshal redis.Reply
func (r *OkReply) ToBytes() []byte {
	return okBytes
}

var theOkReply = new(OkReply)

// MakeOkReply returns a ok reply
func MakeOkReply() *OkReply {
	return theOkReply
}

/**************OK replay end********************/

var nullBulkBytes = []byte("$-1\r\n")

// NullBulkReply is empty string
type NullBulkReply struct{}

// ToBytes marshal redis.Reply
func (r *NullBulkReply) ToBytes() []byte {
	return nullBulkBytes
}

// MakeNullBulkReply creates a new NullBulkReply
func MakeNullBulkReply() *NullBulkReply {
	return &NullBulkReply{}
}

// 空回复
var emptyMultiBulkBytes = []byte("*0\r\n")

// EmptyMultiBulkReply is a empty list
type EmptyMultiBulkReply struct{}

// ToBytes marshal redis.Reply
func (r *EmptyMultiBulkReply) ToBytes() []byte {
	return emptyMultiBulkBytes
}

//	空回复
//
// NoReply respond nothing, for commands like subscribe
type NoReply struct{}

var noBytes = []byte("")

// ToBytes marshal redis.Reply
func (r *NoReply) ToBytes() []byte {
	return noBytes
}
