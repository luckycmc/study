package reply

import (
	"bytes"
	"redis-server/interface/resp"
	"strconv"
)

var (
	nullBulkReplyBytes = []byte("$-1")

	// CRLF is the line separator of redis serialization protocol
	CRLF = "\r\n"
)

/* ---- Bulk Reply ---- */

// BulkReply stores a binary-safe string
type BulkReply struct {
	Arg []byte
}

// MakeBulkReply creates  BulkReply
func MakeBulkReply(arg []byte) *BulkReply {
	return &BulkReply{
		Arg: arg,
	}
}

// 转换成redis协议的东西
// ToBytes marshal redis.Reply
func (r *BulkReply) ToBytes() []byte {
	if len(r.Arg) == 0 {
		return nullBulkReplyBytes
	}
	return []byte("$" + strconv.Itoa(len(r.Arg)) + CRLF + string(r.Arg) + CRLF)
}

/* ---- Multi Bulk Reply ---- */

// MultiBulkReply stores a list of string
type MultiBulkReply struct {
	Args [][]byte
}

// MakeMultiBulkReply creates MultiBulkReply
func MakeMultiBulkReply(args [][]byte) *MultiBulkReply {
	return &MultiBulkReply{
		Args: args,
	}
}

// ToBytes marshal redis.Reply  转换 redis 回复
func (r *MultiBulkReply) ToBytes() []byte {
	argLen := len(r.Args)
	var buf bytes.Buffer
	buf.WriteString("*" + strconv.Itoa(argLen) + CRLF)
	for _, arg := range r.Args {
		if arg == nil {
			buf.WriteString("$-1" + CRLF)
		} else {
			buf.WriteString("$" + strconv.Itoa(len(arg)) + CRLF + string(arg) + CRLF)
		}
	}
	return buf.Bytes()
}

/* ---- Status Reply ---- */

// StatusReply stores a simple status string
type StatusReply struct {
	Status string
}

// MakeStatusReply creates StatusReply
func MakeStatusReply(status string) *StatusReply {
	return &StatusReply{
		Status: status,
	}
}

// ToBytes marshal redis.Reply
func (r *StatusReply) ToBytes() []byte {
	return []byte("+" + r.Status + CRLF)
}

/* ---- Int Reply ---- */

// IntReply stores an int64 number
type IntReply struct {
	Code int64
}

// MakeIntReply creates int reply
func MakeIntReply(code int64) *IntReply {
	return &IntReply{
		Code: code,
	}
}

// ToBytes marshal redis.Reply
func (r *IntReply) ToBytes() []byte {
	return []byte(":" + strconv.FormatInt(r.Code, 10) + CRLF)
}

/* ---- Error Reply ---- */

// ErrorReply is an error and redis.Reply
type ErrorReply interface {
	Error() string
	ToBytes() []byte
}

// StandardErrReply represents handler error
type StandardErrReply struct {
	Status string
}

// ToBytes marshal redis.Reply
func (r *StandardErrReply) ToBytes() []byte {
	return []byte("-" + r.Status + CRLF)
}

func (r *StandardErrReply) Error() string {
	return r.Status
}

// MakeErrReply creates StandardErrReply
func MakeErrReply(status string) *StandardErrReply {
	return &StandardErrReply{
		Status: status,
	}
}

// IsErrorReply returns true if the given reply is error
func IsErrorReply(reply resp.Reply) bool {
	return reply.ToBytes()[0] == '-'
}
