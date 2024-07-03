package main

import "lottery/web/routes"

/**
@抽奖系统入口
**/
func main() {
    
	  r := routes.InitRouter()
	  r.Run(":8080")
}