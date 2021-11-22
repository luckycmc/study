package process

import(
	"fmt"
	"../../common/message"
	"encoding/json"
)

func outputGroupMes(mes *message.Message){//这个地方来的message
    
	   //显示即可
    //1.反序列化
	var smsMes message.SmsMes
	err := json.Unmarshal([]byte(mes.Data), &smsMes)
	if err !=nil{

		 fmt.Println("json.Unmarshal err",err.Error())
		 return
	}
	//显示信息
	info := fmt.Sprintf("用户id:\t%d 对大家说:\t%s",smsMes.UserId,smsMes.Content)
	//content := smsMes.Content
	fmt.Println(info)
	fmt.Println()
}