#include "./include/swoole_tiny_zpw.h"

int server_start();
int servser_create();
int listenFd();
/**********对应的回调函数 start*************/
void onConnect();
void onReceive();
void onClose();
void onFinish();
/**********对应的回调函数 end*************/

/***************回调函数的实现 start**************/
void onConnect()
{
     swTrace("onConnect is comming\n");
}
void onFinish()
{
     swTrace("onFinish is comming\n");
}
void onReceive()
{
      swTrace("onReceive is comming\n");
}
void onClose()
{
     swTrace("onClose is comming\n");
}
/***************回调函数的实现 end**************/
//list fd 函数
int listenFd()
{
    int port = SW_PORT;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		return -1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
		return -2;
	}

	if (listen(sockfd, 5) < 0) {
		return -3;
	}
    return sockfd;
}