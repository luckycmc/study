package resp

//主要是回复

type Reply interface{
	ToBytes() []byte   //转换字节
}