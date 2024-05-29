package resp

//链接的实现
type Connection interface{
	Write([]byte) error
	// used for multi database
	GetDBIndex() int
	SelectDB(int)
}