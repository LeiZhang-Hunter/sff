//
// Created by zhanglei on 19-4-1.
//

#ifndef SFF_SSF_COMMON_H


#include "../sff_common.h"

#endif



void init_socket_lib()
{
    //初始化
    bzero(container_instance.socket_lib,sizeof(sff_socket_lib));
    container_instance.socket_lib->create = sff_socket_create;
    container_instance.socket_lib->socket_errno = 0;
    container_instance.socket_lib->connect = sff_socket_connect;
    container_instance.socket_lib->read = sff_socket_read;
    container_instance.socket_lib->write = sff_socket_write;
    container_instance.socket_lib->loop_work = sff_socket_run;
}

//创建套接字
int sff_socket_create()
{
    if(!container_instance.container_ip)
    {
        return SFF_FALSE;
    }

    if(!container_instance.container_port)
    {
        return SFF_FALSE;
    }

    //现在仅支持ipv4的
    int sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd < 0)
    {
        container_instance.socket_lib->socket_errno = errno;
        return SFF_FALSE;
    }

    container_instance.socket_lib->sockfd = sockfd;
    return  SFF_TRUE;
}

//连接
int sff_socket_connect()
{
    int flags,n,error;
    fd_set read_set,write_set;
    struct timeval tval;
    socklen_t  len;
    int client_fd = container_instance.socket_lib->sockfd;
    flags = fcntl(client_fd,F_GETFL,0);

    //设置套接字编程非阻塞
    fcntl(client_fd,F_SETFL,flags|O_NONBLOCK);

    error = 0;

    struct sockaddr_in client_struct;

    client_struct.sin_family = AF_INET;

    client_struct.sin_port = htons(container_instance.container_port);

    client_struct.sin_addr.s_addr = inet_addr(container_instance.container_ip);

    //对描述符进行链接
    int res = connect(client_fd,(struct sockaddr*)&client_struct,sizeof(client_struct));

    if(res < 0)
    {
        if(errno != EWOULDBLOCK)
            return SFF_FALSE;
    }

    if(res == 0)
    {
        //还原描述符
        fcntl(client_fd,F_SETFL,flags);
        return SFF_TRUE;
    }

    FD_ZERO(&read_set);

    FD_SET(client_fd,&read_set);

    //阻塞3秒
    write_set = read_set;
    tval.tv_sec = 3000;
    tval.tv_usec = 0;

    //select 检测描述符
    if((n = select(client_fd+1,&read_set,&write_set,NULL,&tval)) == 0)
    {
        errno = ETIMEDOUT;
        return  SFF_FALSE;
    }

    //如果说可读或者科协发生了返回
    if(FD_ISSET(client_fd,&read_set) || FD_ISSET(client_fd,&write_set))
    {
        len = sizeof(error);

        if(getsockopt(client_fd,SOL_SOCKET,SO_ERROR,&error,&len) < 0)
            return  SFF_FALSE;
    }

    //还原描述符
    fcntl(client_fd,F_SETFL,flags);
    return SFF_TRUE;
}

//读取
ssize_t sff_socket_read(int sock_fd,const void *vptr,size_t n)
{
    size_t nleft;
    ssize_t nread;
    char* ptr;
    ptr = (char*)vptr;
    nleft = n;

    //如果说还有未读取的字节数，那么就应该继续读取
    while(nleft > 0)
    {
        if((nread = recv(sock_fd,ptr,nleft,0)) < 0)
        {
            //如果说收到信号中断
            if(errno == EINTR)
            {
                nread = 0;
            }else{
                return  SFF_FALSE;
            }
        }else{
            //剩余需要读取的数目
            nleft -= nread;

            //指针偏移，继续向没有读取的位置偏移
            ptr += nread;
        }
    }

    return (n-nleft);
}

//写入
ssize_t sff_socket_write(int sock_fd,const void *vptr,size_t n)
{
    size_t nleft;

    size_t nwrite;

    char *ptr;

    ptr = (char*)vptr;

    nleft = n;

    while(nleft > 0)
    {
        if((nwrite = send(sock_fd,ptr,nleft,0)) < 0)
        {
            if(errno == EINTR)
            {
                nwrite = 0;
            }else{
                return SFF_FALSE;
            }
        }else{
            nleft -= nwrite;

            ptr += nwrite;
        }
    }

    return (n-nleft);
}

int sff_socket_run()
{

}