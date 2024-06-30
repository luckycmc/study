package annual

import (
	"fmt"
	"math/rand"
	"net/http"
	"strings"
	"time"

	"github.com/gin-gonic/gin"
)

var userList []string //定义用户存放
/*
*
年会抽奖首页
*/
func index(ctx *gin.Context) {
	currentCount := len(userList)
	result := fmt.Sprintf("当前总共参与的用户: %d\n", currentCount)
	ctx.String(http.StatusOK, result)
}

// 到收入数据
func gift(ctx *gin.Context) {

	strUsers := ctx.Request.FormValue("users")
	users := strings.Split(strUsers, ",")
	count1 := len(userList)
	for _, u := range users {
		u = strings.TrimSpace(u)
		if len(u) > 0 {
			userList = append(userList, u)
		}
	}
	count2 := len(userList)
	result := fmt.Sprintf("当前总共参与用户的人数: %d,添加去%d 用户", count2, count2-count1)
	ctx.String(http.StatusOK, result)
}

// 抽奖活动
func GetLucky(ctx *gin.Context) {
	count := len(userList)
	var result string
	if count > 1 {

		seed := time.Now().UnixNano()
		index := rand.New(rand.NewSource(seed)).Int31n(int32(count))
		user := userList[index]
		userList = append(userList[0:index], userList[index+1:]...)
		result = fmt.Sprintf("当前中奖用户是:%s,剩余用户数%d", user, len(userList))
	} else if count == 1 {
		user := userList[0]
		userList = userList[:len(userList)-1]
		result = fmt.Sprintf("当前中奖用户是:%s,剩余用户数%d", user, len(userList))
	} else {
		result = "已经没有参与用户了"
	}

	ctx.String(http.StatusOK, result)
}

func StartAnnual() {

	userList = []string{}
	r := gin.Default()
	r.GET("/", index)
	r.GET("/gift", gift)
	r.GET("/getLucky", GetLucky)
	r.Run(":9000")
}
