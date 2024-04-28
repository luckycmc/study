package main

import (
	"fmt"
	"log"
	"net/http"
)

func main() {

	http.HandleFunc("/",indexHander)
	http.HandleFunc("/hello",helloHandler)
	log.Fatal(http.ListenAndServe(":9999",nil))
}
// / index的接口
func indexHander(w http.ResponseWriter,req *http.Request){
	fmt.Fprintf(w, "URL.Path = %q\n", req.URL.Path)
}
// hello 的接口
func helloHandler(w http.ResponseWriter,req *http.Request){
    for k, v := range req.Header {
		fmt.Fprintf(w, "Header[%q] = %q\n", k, v)
	}
	str := "hello world"
	resStr := []byte(str)
	w.Write(resStr)
}