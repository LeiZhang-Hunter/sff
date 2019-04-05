//
// Created by zhanglei on 19-4-1.
//

#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"

#endif

#ifndef	_SYS_EPOLL_H
#include <sys/epoll.h>
#endif

#ifndef SFF_SFF_SOCKET_H
#define SFF_SFF_SOCKET_H

#endif //SFF_SFF_SOCKET_H

#define READBUF 65535


//socket的库
typedef struct _sff_socket_lib{

    int sockfd;

    //创建socket
    int (*create)();

    //链接socket
    int (*connect)();

    int socket_errno;
    //读
    ssize_t (*read)(int sock_fd,const void *vptr,size_t n);

    //写
    ssize_t (*write)(int sock_fd,const void *vptr,size_t n);

    //读取一行
    ssize_t (*readline)(int sock_fd,const void *vptr,size_t maxline);

    //循环工作
    int (*loop_work)();
}sff_socket_lib;

//初始化socket的库
void init_socket_lib();

//创建套接字
int sff_socket_create();

//链接套接字
int sff_socket_connect();

//读取
ssize_t sff_socket_read(int sock_fd,const void *vptr,size_t n);

//写入
ssize_t sff_socket_write(int sock_fd,const void *vptr,size_t n);

int sff_socket_run();

