#include <stdio.h>
#include <string.h>

typedef struct Books {
	
	 char title[50];
	 char author[50];
	 char subject[100];
	 int  book_id;
}Book;

int main(){
	 
	 Book book;
	   //printf( "%p\n", &book);
	  //printf( "%p\n", book.author);
	 strcpy(book.title,"C 教程");   // strcpy 最字符串进行复制  因为 结构体数据  数组名是bai个静态地址值，du不能被修改，\
	                                //如果zhi想把数据存储到其中，‘
	                                // 可以用memcpy（）函数进行内存拷贝，或采用循环进行赋值
	 strcpy( book.author, "Runoob"); 
     strcpy( book.subject, "编程语言");
     // book.subject = "编程语言";
     book.book_id = 12345;  //不能直接赋值结构体字符串 
     
     printf( "书标题 : %s\n", book.title);
     printf( "书作者 : %s\n", book.author);
     printf( "书类目 : %s\n", book.subject);
     printf( "书 ID : %d\n", book.book_id);
    
    return 0;
}
