package wechatshake

/**
 微信摇一摇
 / lucky 只有一个抽奖接口
**/
import "github.com/gin-gonic/gin"

func StartWechatShake() {
	r := gin.Default()
	r.Run(":9000")
}
