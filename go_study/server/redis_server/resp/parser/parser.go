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
	readingMutiLine  bool //是否是多行读取
	expectedArgCount int
	msgType          byte
	args             [][]byte //指令的参数
	bulkLen          int64
}

// 判断解析器是否完成
func (s *readState) finished() bool {
	//要排除0 的情况
	return s.expectedArgCount > 0 && len(s.args) == s.expectedArgCount
}

// 对外接口解析
func ParseStream(reader io.Reader) <-chan *Payload {

	ch := make(chan *Payload)
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
		//判断是不是多行解析模式
		if !state.readingMutiLine { //不是多行模式
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
				if state.msgType == '*' {

					result = reply.MakeMultiBulkReply(state.args)
				} else if state.msgType == '$' {
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
func readLine(bufReader *bufio.Reader, state *readState) ([]byte, bool, error) {

	//1.没有读到预设置的指令长度 直接切就行

	var msg []byte
	var err error

	if state.bulkLen == 0 { //1.按照 \r\n 切分

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
		state.bulkLen = 0
	}

	return msg, false, nil

}

// 解析头
func parseMultiBulkHeader(msg []byte, state *readState) error {

	var err error
	var expectedLine uint64
	expectedLine, err = strconv.ParseUint(string(msg[1:len(msg)-2]), 10, 32)
	if err != nil {
		return errors.New("protpcol error:" + string(msg))
	}
	if expectedLine == 0 {

		state.expectedArgCount = 0
		return nil
	} else if expectedLine > 0 {
		state.msgType = msg[0]
		state.readingMutiLine = true
		state.expectedArgCount = int(expectedLine)
		state.args = make([][]byte, 0, expectedLine)
		return nil
	} else {
		//协议错误
		return errors.New("protpcol error:" + string(msg))
	}

}

// 解析单行头
func parseBulkHeader(msg []byte, state *readState) error {
	var err error
	state.bulkLen, err = strconv.ParseInt(string(msg[1:len(msg)-2]), 10, 64)
	if err != nil {
		return errors.New("protocol error: " + string(msg))
	}
	if state.bulkLen == -1 { // null bulk
		return nil
	} else if state.bulkLen > 0 {
		state.msgType = msg[0]
		state.readingMutiLine = true
		state.expectedArgCount = 1
		state.args = make([][]byte, 0, 1)
		return nil
	} else {
		return errors.New("protocol error: " + string(msg))
	}
}

// +ok-err
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
