package parser

import (
	"bufio"
	"errors"
	"io"
	"redis-server/interface/resp"
	"redis-server/lib/logger"
	"redis-server/resp/reply"
	"runtime/debug"
	"strconv"
	"strings"
)

/*
*

	@redis 解析器

*
*/
type Payload struct {
	Data resp.Reply
	Err  error
}

// 解析器的状态
type readState struct {
<<<<<<< HEAD
	readingMutiLine  bool //是否是多行读取
	expectedArgCount int
	msgType          byte
	args             [][]byte //指令的参数
	bulkLen          int64
=======
	readingMutiLine  bool //是否是读取多行 // 解析单行还是多行数据
	expectedArgCount int  // 期望参数的个数   // 应该读取的参数个数
	msgType          byte  //数据的类型        // 应该读取的参数个数
	args             [][]byte  // 解析后的参数  // 消息内容
	bulkLen          int64  // 数据的长度   // 数据长度
>>>>>>> bed6a7a504e5c463f6c960bd2e09a7709df810cb
}

// 判断解析器是否完成
func (s *readState) finished() bool {
	//要排除0 的情况
	return s.expectedArgCount > 0 && len(s.args) == s.expectedArgCount
}

// 对外接口解析
func ParseStream(reader io.Reader) <-chan *Payload {

	ch := make(chan *Payload)
	//启动一个协成进行协议解析
	go parse0(reader, ch)
	return ch
}

// 解析数据
func parse0(reader io.Reader, ch chan<- *Payload) {

	defer func() {
		if err := recover(); err != nil {
			logger.Error(string(debug.Stack()))
		}
	}()
	//读取缓冲区的数据
	bufReader := bufio.NewReader(reader)
	
	var state readState
	var err error
	var msg []byte
	for {
		var ioErr bool
		msg, ioErr, err = readLine(bufReader, &state)
		
		// 错误处理
		if err != nil {
			//IO有问题
			if ioErr {
				ch <- &Payload{
					Err: err,
				}
				close(ch)
				return
			}
			ch <- &Payload{
				Err: err,
			}
			//重置状态
			state = readState{}
			continue
		}
		//fmt.Println("state.readingMutiLine,",state.readingMutiLine)
		//判断是不是多行解析模式 例如数据  *3\r\n$3\r\nSET\r\n$3\r\nkey\r\n$5\r\nvalue\r\n
		if !state.readingMutiLine { //不是多行模式
			//fmt.Println("msg:  "+string(msg))
			//还没有开启多行解析模式
			if msg[0] == '*' {
				err = parseMultiBulkHeader(msg, &state)
				if err != nil {
					ch <- &Payload{
						Err: errors.New("protocol error :" + string(msg)),
					}
					//重置状态
					state = readState{}
					continue
				}
				//参数为空
				if state.expectedArgCount == 0 {
					// 返回消息
					ch <- &Payload{
						Data: &reply.EmptyMultiBulkReply{},
					}
					state = readState{}
					continue
				}
			} else if msg[0] == '$' { //$3 \r\n  $4\r\nPing\r\n

				err := parseBulkHeader(msg, &state) 
				if err != nil {
					ch <- &Payload{
						Err: errors.New("protocol error :" + string(msg)),
					}
					state = readState{}
					continue
				}
				//空消息
				if state.expectedArgCount == -1 {
					// 返回消息
					ch <- &Payload{
						Data: &reply.NullBulkReply{},
					}
					state = readState{}
					continue
				}
			} else {
				//单行数据
				result, err := parseseSingLineReply(msg)
				ch <- &Payload{
					Data: result,
					Err:  err,
				}
				state = readState{}
				continue
			}
		} else { // 多行模式
            
			err := readBody(msg, &state)
			if err != nil {
				ch <- &Payload{
					Err: errors.New("protocol error :" + string(msg)),
				}
				state = readState{}
				continue
			}
			//读取完成
			if state.finished() {
              
				var result resp.Reply
				if state.msgType == '*' {  //多行字符串
					result = reply.MakeMultiBulkReply(state.args)
				} else if state.msgType == '$' { // 单个字符串
					result = reply.MakeBulkReply(state.args[0])
				}

				ch <- &Payload{
					Data: result,
					Err:  err,
				}
				state = readState{}
				continue
			}
		}

	}
}

// 读取一行数据 $3 \r\n
// 读取一行数据 // 一行一行的读取。读正常的行，以\n分隔。读正文中包含\r\n字符的行时，state.bulkLen加上换行符\r\n（state.bulkLen+2）
func readLine(bufReader *bufio.Reader, state *readState) ([]byte, bool, error) {

	//1.没有读到预设置的指令长度 直接切就行

	var msg []byte
	var err error
    //有两种情况 第一种是 
	if state.bulkLen == 0 { //1.按照 \r\n 切分 其实就是没有预设的长度

		msg, err = bufReader.ReadBytes('\n')
		if err != nil {
			return nil, true, err
		}

		if len(msg) == 0 || msg[len(msg)-2] != '\r' {
			return nil, false, errors.New("protpcol error:" + string(msg))
		}
	} else { //2.之前读到了$ 数字 严格读取字符个数

		msg = make([]byte, state.bulkLen+2)
		_, err := io.ReadFull(bufReader, msg)
		if err != nil {
			return nil, true, err
		}
		//没有按照固定的格式协议
		if len(msg) == 0 || msg[len(msg)-2] != '\r' || msg[len(msg)-1] != '\n' {
			return nil, false, errors.New("protpcol error:" + string(msg))
		}
		//重置预设长度
		state.bulkLen = 0
	}
	//fmt.Println("every line msg:",string(msg))
	return msg, false, nil

}

// 解析头 例如 *3\r\n  *3\r\n$3\r\nSET\r\n$3\r\nkey\r\n$5\r\nvalue\r\n
func parseMultiBulkHeader(msg []byte, state *readState) error {

	var err error
	var expectedLine uint64
	expectedLine, err = strconv.ParseUint(string(msg[1:len(msg)-2]), 10, 32)  // 获取几个参数
	if err != nil {
		return errors.New("protpcol error:" + string(msg))
	}
	if expectedLine == 0 {

		state.expectedArgCount = 0
		return nil
	} else if expectedLine > 0 {  //有几个参数 协议解析器不要停 一直取出来 3
		state.msgType = msg[0]         // 消息的类型 *
		state.readingMutiLine = true   // 多行模式
		state.expectedArgCount = int(expectedLine)   // 几个参数 3
		state.args = make([][]byte, 0, expectedLine) // 参数数据 3
		return nil
	} else {
		//协议错误
		return errors.New("protpcol error:" + string(msg))
	}

}

// 解析单行头 $3 set
func parseBulkHeader(msg []byte, state *readState) error {
	var err error
	state.bulkLen, err = strconv.ParseInt(string(msg[1:len(msg)-2]), 10, 64) //取出$3 的3
	if err != nil {
		return errors.New("protocol error: " + string(msg))
	}
	if state.bulkLen == -1 { // null bulk
		return nil
	} else if state.bulkLen > 0 {
		state.msgType = msg[0]            // $
		state.readingMutiLine = true
		state.expectedArgCount = 1       // 1个
		state.args = make([][]byte, 0, 1) // 1
		return nil
	} else {
		return errors.New("protocol error: " + string(msg))
	}
}

// +ok-err 单行单个数据类型
func parseseSingLineReply(msg []byte) (resp.Reply, error) {

	str := strings.TrimSuffix(string(msg), "\r\n")
	var result resp.Reply
	switch msg[0] {
	case '+':
		result = reply.MakeStatusReply(str[1:])
	case '-':
		result = reply.MakeErrReply(str[1:])
	case ':':
		val, err := strconv.ParseInt(str[1:], 10, 64)
		if err != nil {
			return nil, errors.New("protpcol error:" + string(msg))
		}
		result = reply.MakeIntReply(val)
	}
	return result, nil
}

// 解析 body
func readBody(msg []byte, state *readState) error {

	line := msg[0 : len(msg)-2]
	var err error
	//$3
	if line[0] == '$' {
		state.bulkLen, err = strconv.ParseInt(string(line[1:]), 10, 64)
		if err != nil {
			return errors.New("protpcol error:" + string(msg))
		}
		// $0 \r\n
		if state.bulkLen <= 0 {
			state.args = append(state.args, []byte{})
			state.bulkLen = 0
		}
	} else {
		state.args = append(state.args, line)
	}
	return nil
}
