package main
import "db"
import "fmt"
func main(){
   //db.Dbinit()
	sql:="select * from sci_article limit 1"
	/*row:=db.GetOneRow(sql)
	fmt.Println(row)*/
	//db.GetOneRow()
	db,_:=db.Init()
	data:=db.GetOneRow(sql)
	for k,v:=range data{
		fmt.Println(k)
		fmt.Println(v)
	}
	
}