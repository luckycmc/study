package models

import (
	"blog/config"
	"blog/utils"
	"fmt"
	"strconv"
)

type Comment struct {
	Id         int64  `json:"id" db:"id"`
	Article_id int    `json:"article_id" db:"article_id"`
	Content    string `json:"content" db:"content"`
	User_id    int    `json:"user_id" db:"User_id"`
	Add_time   int    `json:"add_time" db:"add_time"`
	Parent_id  int    `json:"parent_id" db:"parent_id"`
}

//添加评论

func CommentAdd(comment Comment) (int64, error) {
	
    return insertData(comment)
}

// 插入一篇文章
func insertData(comment Comment) (int64, error) {
	sql := "insert into comment(article_id,content,user_id,add_time,parent_id) values (?,?,?,?,?)"

	return utils.ModifyDB(sql,comment.Article_id,comment.Content,comment.User_id,comment.Add_time,comment.Parent_id)
}

//获取相应的评论 更具文章的id
func GetCommentByArticleId(article_id int,page int)([]Comment,error){
	     
	   sqlParams := fmt.Sprintf("limit %d,%d",(page-1) *config.NUM,config.NUM)
       return getCommentByArticleIdData(article_id,sqlParams)
}
//获取数据
func getCommentByArticleIdData(article_id int,sqlParams string)([]Comment,error){

	sql:= "select * from comment where parent_id = 0 and article_id = "  + strconv.Itoa(article_id) + " " + sqlParams
	result,err :=utils.DB.Query(sql)
	 if err != nil {
		fmt.Println(err)
        return nil, err
	 }
	 var commentData []Comment
	 //遍历所有的数据
	 for result.Next(){
		var id int64  
		 id = 0
		 article_id := 0
		 content :=""
		 user_id := 0
		 add_time := 0
		 parent_id := 0
	      //扫描数据
         result.Scan(&id,&article_id,&content,&user_id,&add_time,&parent_id)
		 conm := Comment{id,article_id,content,user_id,add_time,parent_id}
		 commentData = append(commentData, conm)
	 }
	 return commentData,nil
}      
