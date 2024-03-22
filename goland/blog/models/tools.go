package models

import (
	"crypto/md5"
	"fmt"
	"time"
)

// //时间戳间戳转换成日期
func UnixToDate(timestamp int) string {
	t := time.Unix(int64(timestamp), 0)
	return t.Format("2006-01-02 15:04:05")
}

// 日期转换成时间戳 2020-05-02 15:04:05
func DateToUnix(str string) int64 {

	template := "2006-01-02 15:04:05"
	t, err := time.ParseInLocation(template, str, time.Local)
	if err != nil {
		return 0
	}
	return t.Unix()
}

// 获取时间戳
func GetUnix() int64 {
	return time.Now().Unix()
}

// 获取时间
func GetDate() string {
	template := "2006-01-02 15:04:05"
	return time.Now().Format(template)
}

// 获取当前天
func GetDay() string {
	template := "2006-01-02"
	return time.Now().Format(template)
}

// md5 加密
func Md5(str string) string {
	data := []byte(str)
	return fmt.Sprintf("%x\n", md5.Sum(data))
}

// 字符串拼接
func hello(in string) (out string) {
	out = in + "word"
	return
}
