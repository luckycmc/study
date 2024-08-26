package resp

//主要是回复 接受数据也能使用

type Reply interface {
	ToBytes() []byte //转换字节
}
