package tcp

import (
	"context"
	"net"
	"os"
	"os/signal"
	"redis-server/interface/tcp"
	"redis-server/lib/logger"
	"sync"
	"syscall"
)

type Config struct {
	Address string
}

func ListenAndServeWithSignal(cfg *Config, handler tcp.Handler) error {

	closeChan := make(chan struct{})
	//创建信号管道
	sigChan := make(chan os.Signal)
	//监听相应的信号
	signal.Notify(sigChan, syscall.SIGHUP, syscall.SIGQUIT, syscall.SIGTERM, syscall.SIGINT)
	//监听sigChan 启动监听协成 监听信号
	go func() {
		sig := <-sigChan
		switch sig {
		case syscall.SIGHUP, syscall.SIGQUIT, syscall.SIGTERM, syscall.SIGINT:
			closeChan <- struct{}{}
		}
	}()
	//监听server
	listener, err := net.Listen("tcp", cfg.Address)
	if err != nil {
		return err
	}
	logger.Info("start listen...")
	ListenAndServe(listener, handler, closeChan)

	return nil
}

func ListenAndServe(listenner net.Listener, handler tcp.Handler, closeChan <-chan struct{}) {

	//启动一个协成做相应的业务处理
	go func() {
		//监听信号
		<-closeChan
		logger.Info("shutting down")
		_ = listenner.Close()
		_ = handler.Close()
	}()
	// defer  关闭相应的链接
	defer func() {
		_ = listenner.Close()
		_ = handler.Close()
	}()
	//获取上下文信息
	ctx := context.Background()
	var waitDone sync.WaitGroup

	for {
		conn, err := listenner.Accept()
		if err != nil {
			break
		}
		logger.Info("accept link...")
		//加 1
		waitDone.Add(1)
		go func() {
			//执行完后 执行
			defer func() {
				waitDone.Done()
			}()
			handler.Handle(ctx, conn)
		}()
	}
	//wait group 等待
	waitDone.Wait()
}
