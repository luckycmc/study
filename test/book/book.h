#ifndef _BOOK_
#define _BOOK_

#define NAME_SIZE 200
#define PHONE_SIZE 200
#define ADDR_SIZE 200

//定义人物信息结构体
typedef struct PersonInfo{

	  char name[NAME_SIZE];
	  char phone[PHONE_SIZE];
	  char address[ADDR_SIZE];
}PersonInfo;

//定义存放文件的路径
#define FILE_PATH "./data.txt"

//定义枚举数据类型
enum {
  DISPLAY = 1,
  ADD = 2,
  ERASE = 3,
  FIND = 4,
  MODIFY = 5,
  EXIT = 0
};
 
typedef struct AddrBook {
  PersonInfo *data;//内容
  size_t size; //联系人数量
  size_t capacity; //通讯录容量
} AddrBook;
/**
 *  菜单
 * */
void menu();
/**
* @brief 初始化通讯录
*
* @param addr_book
*/
void AddrBookInit(AddrBook* addr_book);
 
/**
* @brief 通讯录中增加条目
*
* @param addr_book
*/
void AddrBookAdd(AddrBook* addr_book);
 
/**
* @brief 删除通讯录中的指定条目
*
* @param addr_book
*/
void AddrBookErase(AddrBook* addr_book);
 
/**
* @brief 按指定规则查找通讯录中特定条目
*
* @param addr_book
*/
void AddrBookFind(AddrBook* addr_book);
 
/**
* @brief 打印通讯录中的所有信息
*
* @param addr_book
*/
void AddrBookDisplay(AddrBook* addr_book);
 
/**
* @brief 修改通讯录的指定条目
*
* @param addr_book
*/
void AddrBookModify(AddrBook* addr_book);
 
/**
* @brief 销毁通讯录
*
* @param addr_book
*/
void AddrBookDestroy(AddrBook* addr_book);
 
/**
* @brief 将通讯录内容保存在文件
*
* @param addr_book
*/
void AddrBookSave(AddrBook* addr_book);
 
/**
* @brief 将文件中的内容加载回内存.
*
* @param addr_book
*/
void AddrBookLoad(AddrBook* addr_book);

//编写对应的方法
#endif // !1