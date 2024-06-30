package ticket

import "github.com/gin-gonic/gin"

//即开记得形
func index(ctx *gin.Context) {

}

//双色球自选型
func GetPrize(ctx *gin.Context) {

}

func StartTicket() {

	r := gin.Default()
	r.GET("/", index)
	r.GET("/getPrize", GetPrize)
	r.Run(":9000")
}
