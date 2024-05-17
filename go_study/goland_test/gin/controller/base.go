package controller

import (
	"github.com/gin-gonic/gin"
	"net/http"
)

type Base struct{

}

func(this *Base) Success(c *gin.Context,str string){
	if str == ""{
		c.String(http.StatusOK,"success....")
	}else{
		c.String(http.StatusOK,"success----"+str)
	}
   
}

func(this *Base) Fail(c *gin.Context){
    c.String(http.StatusOK,"error....")
}