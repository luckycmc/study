package reply

type PongReply struct {
}

var pongbytes = []byte("+PONG\r\n")

func (r PongReply) ToBytes() []byte {

	return pongbytes
}

func MakePongReply() *PongReply {

	return &PongReply{}
}

/*************回复 OK start*********/
type OkReply struct {
}

var okBytes = []byte("+OK\r\n")

func (r *OkReply) ToBytes() []byte {
	return okBytes
}

var theOkReply = new(OkReply)

func MakeOkReply() *OkReply {
	return theOkReply
}

/*************回复 OK end*********/