/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ADLIST_H__
#define __ADLIST_H__

/* Node, List, and Iterator are the only data structures used currently. */
/**
 * 双端链表节点
 * 
 */
typedef struct listNode {
    struct listNode *prev;  //前置节点
    struct listNode *next;  //后置节点
    void *value;           //对应的节点的值
} listNode;
/****
 * 双端链表的迭代器
 * */
typedef struct listIter {
    listNode *next;
    int direction;
} listIter;
/*listNode 列表*/
typedef struct list {
    listNode *head;     //指向头结点
    listNode *tail;     //指向尾部节点
    /* 下面3个方法为所有结点公用的方法，分别在相应情况下回调用 */
    //复制函数指针
    void *(*dup)(void *ptr);
    //释放函数指针
    void (*free)(void *ptr);
    //匹配函数指针
    int (*match)(void *ptr, void *key);
    //链表的长度
    unsigned long len;
} list;

/* Functions implemented as macros */
/* 宏定义了一些基本操作 */
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))        // //列表的复制方法的设置
#define listSetFreeMethod(l,m) ((l)->free = (m))      //列表的释放方法的设置
#define listSetMatchMethod(l,m) ((l)->match = (m))     //列表的匹配方法的设置

#define listGetDupMethod(l) ((l)->dup)             //列表的复制方法的获取
#define listGetFree(l) ((l)->free)                 //列表的释放方法的获取
#define listGetMatchMethod(l) ((l)->match)          //列表的匹配方法的获取

/* Prototypes */
/* 定义了方法的原型 */
list *listCreate(void);                             //创建list的链表
void listRelease(list *list);                       //列表的释放
list *listAddNodeHead(list *list, void *value);     //添加列表头结点
list *listAddNodeTail(list *list, void *value);     //添加列表尾结点
list *listInsertNode(list *list, listNode *old_node, void *value, int after);    //某位置上插入及结点
void listDelNode(list *list, listNode *node);               //列表上删除给定的结点
listIter *listGetIterator(list *list, int direction);       //获取列表给定方向上的迭代器
listNode *listNext(listIter *iter);                         //获取迭代器内的下一结点
void listReleaseIterator(listIter *iter);                   //释放列表迭代器 
list *listDup(list *orig);                                  //列表的复制
listNode *listSearchKey(list *list, void *key);             //关键字搜索具体结点
listNode *listIndex(list *list, long index);                //下标索引具体的结点
void listRewind(list *list, listIter *li);                  // 重置迭代器为方向从头开始
void listRewindTail(list *list, listIter *li);              //重置迭代器为方向从尾部开始 
void listRotate(list *list);                               //列表旋转操作，方法名说的很玄乎，具体只能到实现里去看了

/* Directions for iterators */
/* 定义2个迭代方向，从头部开始往尾部，第二个从尾部开始向头部 */
#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */
