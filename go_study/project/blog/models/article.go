package models

import (
	"blog/config"
	"blog/utils"
	"fmt"
	"strconv"
)

type Article struct {
	Id         int      `json:"id"`
	Title      string   `json:"title"`
	Tags       string   `json:"tags"`
	Short      string   `json:"short"`
	Content    string   `json:"content"`
	Author     string   `json:"author"`
	Createtime int64    `json:"createtime"`
}

/****数据处理***/
func AddArticle(article Article) (int64, error) {
    
	 return insertArticle(article)
}

//插入一篇文章

func insertArticle(article Article) (int64, error) {
    
	sql := "insert into article(title,tags,short,content,author,createtime) values(?,?,?,?,?,?)" //对应的sql 语句
	return utils.ModifyDB(sql,article.Title, article.Tags, article.Short, article.Content, article.Author, article.Createtime)
}

//-----------查询文章---------

//根据页码查询文章
func FindArticleWithPage(page int) ([]Article, error) {
    page-- //使用到分页所以 会 page 先进行减去1
    //fmt.Println("---------->page", page)
    //从配置文件中获取每页的文章数量
    return QueryArticleWithPage(page, config.NUM)
}

/**
分页查询数据库
limit分页查询语句，
    语法：limit m，n

    m代表从多少位开始获取，与id值无关
    n代表获取多少条数据

注意limit前面咩有where
 */
func QueryArticleWithPage(page, num int) ([]Article, error) {
	// 拼接相应的sql 语句
    sql := fmt.Sprintf("limit %d,%d", page, num)
	//fmt.Println(sql)
    return QueryArticlesWithCon(sql)
}

func QueryArticlesWithCon(sql string) ([]Article, error) {
    sql = "select id,title,tags,short,content,author,createtime from article order by id desc " + sql
	//执行sql 语句
    rows, err := utils.DB.Query(sql)
    if err != nil {
		fmt.Println(err)
        return nil, err
    }
	
    var artList []Article
	//遍历出所有的数据 也就是结果集
    for rows.Next() {
        id := 0
        title := ""
        tags := ""
        short := ""
        content := ""
        author := ""
        var createtime int64
        createtime = 0
		// 获取相应的数据
        rows.Scan(&id, &title, &tags, &short, &content, &author, &createtime)
		//赋值到结构图中
        art := Article{id, title, tags, short, content, author, createtime}
		//把数据都追加到切片中
        artList = append(artList, art)
    }
    return artList, nil
}

//查询文章的总数
func QueryArticleRowNum() int{
	 sql := "select count(*) from article"
	 row := utils.DB.QueryRow(sql)
	 num :=0;
	 //获取相应的结果集
	 row.Scan(&num)
	 return num
}

//获取文章详情信息
func QueryArticleDetails(id int) Article{
    
	  sql := "select * from article where id=" + strconv.Itoa(id)
	  row := utils.DB.QueryRow(sql)
	  title := ""
	  tags := ""
	  short := ""
	  content := ""
	  author := ""
	  var createtime int64
	  createtime = 0
	  //获取相应的数据 
	  row.Scan(&id,&title,&tags,&short,&content,&author,&createtime)
	  // 赋值到相应的结构体中
	  art := Article{id,title,tags,short,content,author,createtime}
	  return art
}