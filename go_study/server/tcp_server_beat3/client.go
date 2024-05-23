package main

import (
	"fmt"
	"net"
	"os"
)
 
func main() {
    
    
    // 尝试建立TCP连接
    conn, err := net.Dial("tcp", "127.0.0.1:9000")
    if err != nil {
        fmt.Fprintf(os.Stderr, "Error dialing: %s\n", err)
        os.Exit(1)
    }
    defer conn.Close() // 确保连接在结束时关闭
    
    // 发送数据到服务器
    _, err = conn.Write([]byte("user:zpw|pass:123456|type:2\n"))
    if err != nil {
        fmt.Fprintf(os.Stderr, "Error writing to socket: %s\n", err)
        os.Exit(1)
    }
    
    // 读取服务器响应
    buf := make([]byte, 1024)
    n, err := conn.Read(buf)
    if err != nil {
        fmt.Fprintf(os.Stderr, "Error reading from socket: %s\n", err)
        os.Exit(1)
    }
    
    // 打印服务器响应
    fmt.Printf("Received: %s\n", buf[:n])
}
