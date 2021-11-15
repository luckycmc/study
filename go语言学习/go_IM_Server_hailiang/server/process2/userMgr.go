package process2
import(
	"fmt"
)

//UserMgr 全局变量
var(
	userMgr *UserMgr
)
//结构体
type UserMgr struct {
	  
	   onlineUsers map[int]*UserProcess
}
//初始化数据
func init(){
	userMgr = &UserMgr{
		onlineUsers: make(map[int]*UserProcess),
	}
}

//完成onlinemapUsers的添加
func(this *UserMgr) AddOnlineUsers(up *UserProcess) {
     
	   this.onlineUsers[up.UserId] = up
} 

//完成onlinemapUsers的添加
func(this *UserMgr) DelOnlineUsers(userId int) {
     
	 delete(this.onlineUsers, userId)
} 

//返回当前所有在线的用户
func(this *UserMgr) GetOnlineUsers() map[int]*UserProcess {
	  
	   return this.onlineUsers
}
//返回对应的process
func(this *UserMgr) GetOnlineUserById(userId int) (up *UserProcess,err error) {
        
	   //从map中取出一个值,带检测方式
	   up,ok := this.onlineUsers[userId]
	   if !ok {  //说明当前要查找的用户不在线
             
		    err = fmt.Errorf("用户%d 不存在",userId)
			return
	   }
	   return
}

