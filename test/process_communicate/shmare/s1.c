#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
/**
  多个进程可以把一段内存映射到自己的进程空间，以此来实现数据的共享及传输，
  这也是所有进程间通信方式最快的一种，共享内存是存在于内核级别的一种资源。
  ipcs -m ，其中 -m 是 memory 的意思 。
 * 
 */