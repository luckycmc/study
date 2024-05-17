#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <error.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include <stdlib.h>
/**
 * 
 * socketpair()函数用于创建一对无名的、相互连接的套接子。 
   如果函数成功，则返回0，创建好的套接字分别是sv[0]和sv[1]；否则返回-1，错误码保存于errno中。
   基本用法： 
   1. 这对套接字可以用于全双工通信，每一个套接字既可以读也可以写。例如，可以往sv[0]中写，从sv[1]中读；
       或者从sv[1]中写，从sv[0]中读； 
   2. 如果往一个套接字(如sv[0])中写入后，再从该套接字读时会阻塞，只能在另一个套接字中(sv[1])上读成功； 
   3. 读、写操作可以位于同一个进程，也可以分别位于不同的进程，如父子进程。如果是父子进程时，一般会功能分离，
        一个进程用来读，一个用来写。因为文件描述副sv[0]和sv[1]是进程共享的，所以读的进程要关闭写描述符,
         反之，写的进程关闭读描述符。

 * @return int 
 */
const char *str = "SOCKET PAIR TEST DATA";

int main()
{
     char buf[128]={0};
     int socket_pair[2];

     //创建进程管道
     if (socketpair(AF_UNIX, SOCK_STREAM,0,socket_pair) == -1)
     {
          printf("Error, socketpair create failed, errno(%d): %s\n", errno, strerror(errno));
          return EXIT_FAILURE; 
     }
     //从管道中写数据
     int size = write(socket_pair[0],str,strlen(str));
     //从管道中读数据
     read(socket_pair[1],buf,size);
     //打印输出读取的数据
     printf("Read result: %s\n",buf);

     return 1;
    
}