package controller

import (
	"github.com/gin-gonic/gin"
	"net/http"
)

type Api struct{

}

func(this *Api) ApiIndex(r *gin.Context){
     
       r.JSON(http.StatusOK,gin.H{
             "name":"zpw",
			 "age":34,
	   })
}

