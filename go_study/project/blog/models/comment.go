package models

import "blog/utils"

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