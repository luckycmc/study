#ifndef PHP_SERVER_H
# define PHP_SERVER_H

# include "php_tinyswoole.h"

#define LISTENQ 10
#define MAX_BUF_SIZE 1024

int saySocketCreate();
int saySocketBind(int sock,char *host, int port);

#endif	/* PHP_SAY_H */