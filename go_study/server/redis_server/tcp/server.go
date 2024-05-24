package tcp

import (
	"context"
	"net"
	"redis-server/interface/tcp"
	"redis-server/lib/logger"
)

type Config struct {
	Address string
}

func ListenAndServeWithSignal(cfg *Config, handler tcp.Handler) error {
	 closeChan := make(chan struct{})
	 //监听server
     listener ,err := net.Listen("tcp",cfg.Address)
	 if err != nil {
		  return err
	 }
	 logger.Info("start listen...")
     ListenAndServe(listener,handler,closeChan)
	 
	 return nil
}

func ListenAndServe(listenner net.Listener,handler tcp.Handler,closeChan <- chan struct{}){
        
	ctx := context.Background()
	  for{
		  conn,err := listenner.Accept()
		  if err != nil {
			   continue
		  }
		  logger.Info("accept link...")
		  go func(){
               handler.Handle(ctx,conn)
		  }()
	  }
}