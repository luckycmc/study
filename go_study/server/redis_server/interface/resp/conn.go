package resp

//链接的实现
type Connection interface{
	Write([]byte) error
	GetDBIndex() int
	SelectDB(int)
}