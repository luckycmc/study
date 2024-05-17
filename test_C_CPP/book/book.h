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
#endif // !1��应的数据
       printf("\n====================通讯录=========================\n");
       printf("|   姓名   |   号码   |   地址   |\n");
       printf("|----------------|---------------|----------------|\n");
       size_t i =0;
       for (i = 0; i < addr_book->size; i++)
       {   
           //打印对应数组下面的数据
           printf("|   %s   |   %s   |   %s   |\n", 
           addr_book->data[i].name, addr_book->data[i].phone, addr_book->data[i].address);
           printf("|----------------|---------------|----------------|\n");
       }
       
       
}
//通讯录初始化
void AddrBookInit(AddrBook *addr_book)
{
      if (addr_book == NULL)
      {
          return;
      }
      addr_book->size = 0;
      addr_book->capacity = 1;
      addr_book->data  = (PersonInfo*)malloc(sizeof(PersonInfo)*addr_book->capacity);
}
//内存扩容
void AddrBookRelloc(AddrBook *addr_book)
{
     if (addr_book ==NULL)
     {
         return;
     }
     //用的是堆内存 heap
     addr_book->capacity = addr_book->capacity * 2 +1; //内存不够，扩大容量
     PersonInfo *new_data = (PersonInfo*)malloc(sizeof(PersonInfo)*addr_book->capacity);
     //保留之前就内存的数据
     size_t i =0;
     for (; i < addr_book->size; i++)
     {
         new_data[i] = addr_book->data[i];
     }
     free(addr_book->data);
     addr_book->data = new_data;
}
//添加联系人
void AddrBookAdd(AddrBook * addr_book)
{
     
       if (addr_book == NULL)
       {
            return;// 指针节点为空的处理
       }
       //检查内存容量是否足够大 不够大则扩容
       if (addr_book->size >= addr_book->capacity)
       {
            AddrBookRelloc(addr_book);
       }
       
       //获取书的容量的大小
       size_t cur = addr_book->size;
       ++addr_book->size; //书的容量加一
       /********************添加显示的内容 start*********************************/
        printf("添加联系人\n");
        printf("输入姓名：");
        scanf("%s", &addr_book->data[cur].name);
        printf("输入号码：");
        scanf("%s", &addr_book->data[cur].phone);
        printf("输入地址：");
        scanf("%s", &addr_book->data[cur].address);
        printf("添加结束!\n");
       /********************添加显示的内容 end*********************************/
}
//将文件中的内容加载回内存
void AddrBookLoad(AddrBook *addr_book)
{
    if (addr_book = NULL){
        printf("通讯录为空！\n");
        return;
    }
    
    FILE* fp = fopen(FILE_PATH,"r");
    if (fp = NULL){
        printf("打开文件失败!\n");
        return;
    }
    while (!feof(fp))
    {
        if (addr_book->size >= addr_book->capacity){
            AddrBookRelloc(addr_book);
        }
        size_t cur = addr_book->size;
        fscanf(fp, "%s%s%s\n", addr_book->data[cur].name, addr_book->data[cur].phone, addr_book->data[cur].address);
        addr_book->size++;
    }
    fclose(fp);
}