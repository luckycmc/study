package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func main() {
	//获取请求页面
	response, err := http.Get("https://www.liwenzhou.com/")
	if err != nil {
		fmt.Println("get failed, err:", err)
		return
	}
    //fmt.Println(response.Body)
	defer response.Body.Close()
	//读取页面内容
	body, err := ioutil.ReadAll(response.Body)
	//转换成字符串
	fmt.Println(string(body))
}