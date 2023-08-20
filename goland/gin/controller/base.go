package controller

import (
	"github.com/gin-gonic/gin"
	"net/http"
)

type Base struct{

}

func(this *Base) Success(c *gin.Context){
   c.String(http.StatusOK,"success....")
}

func(this *Base) Fail(c *gin.Context){
    c.String(http.StatusOK,"error....")
}