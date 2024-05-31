package database

import "strings"

// 记录指令和command直接的关系
var cmdTable = make(map[string]*command)

type command struct {
	exector ExecFunc
	arity   int
}

// 注册指令的方法
func RegisterCommand(name string, exector ExecFunc, arity int) {

	name = strings.ToLower(name)
	cmdTable[name] = &command{
		exector: exector,
		arity:   arity,
	}
}
