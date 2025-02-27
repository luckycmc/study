#ifndef _TCP_CHAT
#define _TCP_CHAT

#ifndef _TCP_CHAT
#define _TCP_CHAT

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
 #include <string.h>


#define SERVER_PORT 8888


//在线用户
struct ONLINE {
    
     int fd; // -1
     int flage;// registerd or no
     char name[32];
     char password[32];
};
//用户的最大数
#define MAX_USER_NUM 64

//C/S通信的结构体
struct protocol
{
    int cmd;
    int state;
    char name[32];
    char data[64];
}

/*cmd*/
#define BROADCAST 0X00000001
#define PRIVATE 0X00000002
#define REGISTE 0X00000004
#define LOGIN 0X00000008
#define ONLINEUSER     0X00000010
#define LOGOUT     0X00000020

/*return code*/
#define OP_OK    0X80000000
#define ONLINEUSER_OK    0X80000001
#define ONLINEUSER_OVER  0X80000002
#define NAME_EXIST 0X80000003
#define NAME_PWD_NMATCH 0X80000004
#define USER_LOGED 0X80000005
#define USER_NOT_REGIST 0X80000006

 #endif