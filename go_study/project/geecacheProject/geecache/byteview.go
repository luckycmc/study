package geecache

type ByteView struct {
	b []byte
}

// 获取切片的长度
func (v ByteView) Len() int {
	return len(v.b)
}

func (v ByteView) ByteSlice() []byte {

	return cloneBytes(v.b)
}

// 转换字符串
func (v ByteView) String() string {
	return string(v.b)
}

// clone 函数  b 是只读的，使用 ByteSlice() 方法返回一个拷贝，防止缓存值被外部程序修改。
func cloneBytes(b []byte) []byte {
	c := make([]byte, len(b))
	copy(c, b)
	return c
}