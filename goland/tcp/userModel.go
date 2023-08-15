package main

type User struct {
    UserId   int    `db:"user_id"`
    Username string `db:"username"`
}

func(this *User) InsertData() int{
    
}


func(this *User) Data() int{
    
}