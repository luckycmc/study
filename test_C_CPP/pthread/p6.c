/***************
 * 本节作为指引性介绍，linux下线程的属性是可以根据实际项目需要，进行设置，之前我们讨论的线程都是采用线程的默认属性，
 * 默认属性已经可以解决绝大多数开发时遇到的问题。
 * 如我们对程序的性能提出更高的要求那么需要设置线程属性，
 * 比如可以通过设置线程栈的大小来降低内存的使用，增加最大线程个数。
 * */
//什么是线程 start
/****
     LWP：light weight process 轻量级的进程，本质仍是进程(在Linux环境下)

    进程：独立地址空间，拥有PCB

    线程：也有PCB，但没有独立的地址空间(共享)

    区别：在于是否共享地址空间。 独居(进程)；合租(线程)。

    Linux下： 线程：最小的执行单位

   进程：最小分配资源单位，可看成是只有一个线程的进程

   2. Linux内核线程实现原理

    （1）线程实现原理

    类Unix系统中，早期是没有“线程”概念的，80年代才引入，借助进程机制实现出了线程的概念。因此在这类系统中，进程和线程关系密切。

    1）轻量级进程(light-weight process)，也有PCB，创建线程使用的底层函数和进程一样，都是clone

    2）从内核里看进程和线程是一样的，都有各自不同的PCB，但是PCB中指向内存资源的三级页表是相同的

    3）进程可以蜕变成线程

    4）线程可看做寄存器和栈的集合

    5）在linux下，线程最是小的执行单位；进程是最小的分配资源单位
 * */
#include <stdio.h>

typedef struct
{
    int etachstate; //线程的分离状态
    int schedpolicy; //线程调度策略
    struct sched_param schedparam; //线程的调度参数
    int inheritsched; //线程的继承性
    int scope; //线程的作用域
    size_t guardsize; //线程栈末尾的警戒缓冲区大小
    int stackaddr_set; //线程的栈设置
    void* stackaddr; //线程栈的位置
    size_t stacksize; //线程栈的大小

} pthread_attr_t;   // 察看LWP号：ps -Lf pid 查看指定线程的lwp号。