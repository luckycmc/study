package controller

import (
	"github.com/gin-gonic/gin"
	"net/http"
)
type Admin struct{

}

func(this *Admin) AdminIndex(r *gin.Context){

    r.String(http.StatusOK,"this is admin module")
}