package ArrayList

//接口
type List interface{
	Size() int //数组的大小
	Get(index int)(interface{},error)  //抓取的第几个元素
	Set(index int,newval interface{} ) error //设置修改数据
	Insert(index int,newval interface{}) error //插入数据
	Append(newval interface{}) error //追加
	Clear()  //清空
	Delete(index int) error //删除
	String() String  //返回一个字符串

}

//数据结构 字符串 整数 实数
type ArrayList struct{

	dataStore [] interface{}  //数组的存储
    theSize int
}
//初始化数组
func NewArrayList() *ArrayList{
   
	  list := new(ArrayList)
	  list.dataStore = make([]interface{}, 0,10)  //开辟10个空间
	  list.theSize = 0
	  return list
}

//求数组的大小
func (list *ArrayList) Size() int {
	  
	  return list.theSize  //返回数组的大小
}
//获取数组中的元素
func (list *ArrayList) Get(index int) (interface{},error){

	 if index < 0 || index >= list.theSize {
		  
		   return nil,errors.New("索引越界")
	 }
	 return list.dataStore[index]
}


