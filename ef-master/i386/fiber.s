# Copyright (c) 2018-2020 The EFramework Project
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

.equ FIBER_STACK_UPPER_OFFSET, 8
.equ FIBER_STACK_PTR_OFFSET, 16
.equ FIBER_STATUS_OFFSET, 20
.equ FIBER_PARENT_OFFSET, 24
.equ FIBER_SCHED_OFFSET, 28

.equ SCHED_CURRENT_FIBER_OFFSET, 0

.equ FIBER_STATUS_EXITED, 0
.equ FIBER_STATUS_INITED, 1

.global ef_fiber_internal_swap
.global ef_fiber_internal_init

.text

ef_fiber_internal_swap:
mov %esp,%eax
push %edx
push %ecx
push %ebx
push %ebp
push %esi
push %edi
pushfl
mov 8(%eax),%edx
mov %esp,(%edx)
mov 4(%eax),%esp
mov 12(%eax),%eax
_ef_fiber_restore:
popfl
pop %edi
pop %esi
pop %ebp
pop %ebx
pop %ecx
pop %edx
ret

_ef_fiber_exit:
pop %edx
mov $FIBER_STATUS_EXITED,%ecx
mov %ecx,FIBER_STATUS_OFFSET(%edx)
mov FIBER_PARENT_OFFSET(%edx),%ecx
mov FIBER_SCHED_OFFSET(%edx),%edx
mov %ecx,SCHED_CURRENT_FIBER_OFFSET(%edx)
mov FIBER_STACK_PTR_OFFSET(%ecx),%esp
jmp _ef_fiber_restore

ef_fiber_internal_init:
push %ebp
mov %esp,%ebp
push %edx
push %ecx
mov 8(%ebp),%edx
mov $FIBER_STATUS_INITED,%eax
mov %eax,FIBER_STATUS_OFFSET(%edx)
mov %edx,%ecx
mov FIBER_STACK_UPPER_OFFSET(%edx),%edx
mov %ecx,-4(%edx)
mov 16(%ebp),%eax
mov %eax,-8(%edx)
lea _ef_fiber_exit,%eax
mov %eax,-12(%edx)
mov 12(%ebp),%eax
mov %eax,-16(%edx)
xor %eax,%eax
mov %eax,-20(%edx)
mov %eax,-24(%edx)
mov %eax,-28(%edx)
mov %edx,-32(%edx)
mov %eax,-36(%edx)
mov %eax,-40(%edx)
mov %eax,-44(%edx)
mov %edx,%eax
sub $44,%eax
pop %ecx
pop %edx
mov %ebp,%esp
pop %ebp
ret

  75%     12
  80%     12
  90%     13
  95%     14
  98%     15
  99%     20
 100%     24 (longest request)
```

## 目录结构 ##

```
├-- amd64
│   └-- fiber.s   // 汇编实现协程初始化与切换等底层逻辑
├-- i386
│   └-- fiber.s
├-- util
├-- coroutine.h
├-- coroutine.c   // 实现协程池，简化了协程的管理
├-- fiber.h
├-- fiber.c       // 实现了协程，提供核心API
├-- framework.h
├-- framework.c   // 框架层，封装了事件循环，实现了基于IO的协程调度
├-- epoll.c
├-- epollet.c     // edge triger
├-- kqueue.c
├-- poll.c        // 基本上所有Unix系统都会支持poll
├-- poll.h
├-- port.c        // event port
├-- main.c
├-- Makefile
└-- Makefile.i386
```

## 示例浅析 ##

1. 首先要进行框架初始化，包括协程池初始化与IO多路复用初始化工作。
2. 然后创建用于监听端口的socket并加入到框架中存储监听类型socket的链表中，并指定业务处理入口。
3. 最后运行框架，开始IO多路复用的事件循环就可以了。

以下示例来自`main.c`：

```
int main(int argc, char *argv[])
{
    // 1. 初始化框架
    // 协程池初始化，需要指定协程池规模，协程栈大小
    // IO多路复用初始化
    if (ef_init(&efr, 64 * 1024, 256, 512, 1000 * 60, 16) < 0) {
        return -1;
    }

    ......

    // 2. 创建监听socket
    // 监听8080端口
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        return -1;
    }
    struct sockaddr_in addr_in = {0};
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(8080);
    int retval = bind(sockfd, (const struct sockaddr *)&addr_in, sizeof(addr_in));
    if(retval < 0)
    {
        return -1;
    }
    listen(sockfd, 512);

    // 把socket加入监听socket链表
    // 框架支持多个监听socket分别监听不同端口，所以先放入链表，框架运行起来后会一并处理
    // 需要指定业务处理入口，此处为forward_proc
    // 新建立的连接会交给一个协程，forward_proc便是这些协程的执行入口
    ef_add_listen(&efr, sockfd, forward_proc);

    ......

    // 3. 运行框架，开启IO多路复用事件循环
    return ef_run_loop(&efr);
}
```

接下来我们要做的就是实现forward_proc等业务处理函数，在其中使用框架包装好的IO操作函数，就可以按照常规业务逻辑来编写，完全不用关心协程切换与IO事件注册。

```
// 将8080端口接收到的GET请求转发到80端口
long forward_proc(int fd, ef_routine_t *er)
{
    char buffer[BUFFER_SIZE];
    // 读请求，理论上对于GET一次read应该就可以
    ssize_t r = ef_routine_read(er, fd, buffer, BUFFER_SIZE);
    if(r <= 0)
    {
        return r;
    }

    // 建立到80端口的连接
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr_in = {0};
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(80);
    int ret = ef_routine_connect(er, sockfd, (const struct sockaddr *)&addr_in, sizeof(addr_in));
    if(ret < 0)
    {
        return ret;
    }

    // 将读取到的请求体发送到80端口
    ssize_t w = ef_routine_write(er, sockfd, buffer, r);
    if(w < 0)
    {
        goto exit_proc;
    }

    while(1)
    {
        // 从80端口循环读取响应数据
        r = ef_routine_read(er, sockfd, buffer, BUFFER_SIZE);
        if(r <= 0)
        {
            break;
        }
        ssize_t wrt = 0;

        // 将响应数据写给请求方，循环确保完全写入
        while(wrt < r)
        {
            w = ef_routine_write(er, fd, &buffer[wrt], r - wrt);
            if(w < 0)
            {
                goto exit_proc;
            }
            wrt += w;
        }
    }
exit_proc:
    ef_routine_close(er, sockfd);
    return ret;
}
```
