package main

import (
	"fmt"
	"net/http"
)

func sayHello(w http.ResponseWriter,r *http.Request){
     fmt.Fprintln(w,"<h1>Hello http server</h1>")
}

func main() {

	http.HandleFunc("/",sayHello)
	err := http.ListenAndServe(":9000",nil)
	if err!=nil {
		fmt.Printf("http server failed, err:%v\n", err)
        return
	}
	fmt.Println("server is running...")
}