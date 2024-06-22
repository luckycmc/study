package connection

import (
	"net"
	"redis-server/lib/sync/wait"
	"sync"
	"time"
)

// 链接层
type Connection struct {
	conn          net.Conn
	waittingReply wait.Wait
	mu            sync.Mutex
	selectedDB    int
}

// 获取conn
func NewConn(conn net.Conn) *Connection {
	return &Connection{
		conn: conn,
	}
}

// 获取远程地址
func (c *Connection) RemoteAddr() net.Addr {
	return c.conn.RemoteAddr()
}

// 关闭客户端
func (c *Connection) Close() error {

	c.waittingReply.WaitWithTimeout(10 * time.Second)
	_ = c.conn.Close()
	return nil
}

// 数据 返回给客户端
func (c *Connection) Write(bytes []byte) error {

	if len(bytes) == 0 {
		return nil
	}
	//枷锁
	c.mu.Lock()
	c.waittingReply.Add(1)
	defer func() {
		c.waittingReply.Done()
		c.mu.Unlock()
	}()
	_, err := c.conn.Write(bytes)
	return err
}

// 得到数据库
func (c *Connection) GetDBIndex() int {
	return c.selectedDB
}

// 选择数据库
func (c *Connection) SelectDB(dbNum int) {
	c.selectedDB = dbNum
}
