package main

type User struct {
    UserId   int    `db:"user_id"`
    Username string `db:"username"`
}
//添加数据
func(this *User) InsertData() int{
    
}

//查询数据
func(this *User) Data() int{
    
}