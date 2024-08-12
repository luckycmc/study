package main

import "goredis-main/app"

func main() {
	// server 层面的初始化
	server, err := app.ConstructServer()
	if err != nil {
		panic(err)
	}
    // 启动应用
	app := app.NewApplication(server, app.SetUpConfig())
	//终止服务
	defer app.Stop()

	if err := app.Run(); err != nil {
		panic(err)
	}
}
