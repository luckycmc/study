package utils

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

/*
{
	"code": 10000, // 程序中的错误码
	"msg": xx,     // 提示信息
	"data": {},    // 数据
}

*/

type ResponseData struct{
	Code ResCode        `json:"code"`
	Msg interface{}     `json:"msg"`
	Data interface{}    `json:"data"`
}
/**
@返回错误信息
**/
func ResponseError(c *gin.Context,code ResCode){
      c.JSON(http.StatusOK,&ResponseData{
		   Code: code,
		   Msg: code.Msg(),
		   Data: nil,
	  })
}

/**
@自己定义错误信息
**/

func ResponseErrorWithMsg(c *gin.Context,code ResCode,msg interface{}){
	 c.JSON(http.StatusOK,&ResponseData{
		Code: code,
		Msg: msg,
		Data: nil,
	 })
}

/**
@返回成功的信息
**/

func ResponseSuccess(c *gin.Context,code ResCode,data interface{}){
       
	  c.JSON(http.StatusOK,&ResponseData{
		  Code: code,
		  Msg: CodeSuccess.Msg(),
		  Data: data,
	  })
} 