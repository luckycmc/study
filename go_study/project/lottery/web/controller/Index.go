package controller

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

type Index struct {
}

func (c Index) Index(r *gin.Context) {
        
	 r.HTML(http.StatusOK,"default/default.html",gin.H{
		 "ok":"ok",
	 })
}