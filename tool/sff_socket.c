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
    container_instance.socket_lib->setnoblock = setnoblock;
    container_instance.socket_lib->setblock = setblock;
    container_instance.socket_lib->close = sff_close;
    container_instance.socket_lib->reconnect = sff_reconnect;
}

//把描述符设置为非阻塞
int setnoblock(int fd)
{
    int flags;
    flags = fcntl(fd,F_GETFL);
    fcntl(fd,F_SETFL,flags|O_NONBLOCK);
}

//设置为阻塞模式
int setblock(int fd)
{
    int flags;
    flags = fcntl(fd,F_GETFL);
    flags &= ~O_NONBLOCK;
    fcntl(fd,F_SETFL,flags);
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

    if(container_instance.recv_buf > 0)
    {
        if(setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,&container_instance.recv_buf,sizeof(int)) < 0)
        {
            php_error_docref(NULL, E_WARNING, "set recv buffer error");
        }
    }

    if(container_instance.send_buf > 0)
    {
        if(setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,&container_instance.send_buf,sizeof(int)) < 0)
        {
            php_error_docref(NULL, E_WARNING, "set send buffer error");
        }
    }

    return  SFF_TRUE;
}

//当断线重连的时候进行重连
void sff_reconnect()
{

    int client_fd = container_instance.socket_lib->sockfd;

    //关闭最初的描述符
    container_instance.socket_lib->close(client_fd);

    int res;

    while((res = container_instance.socket_lib->connect()) == SFF_FALSE)
    {
        container_instance.socket_lib->close(client_fd);
        container_instance.socket_lib->create();
        php_error_docref(NULL, E_WARNING, "connect server error");
        sleep(1);
    }

}

SFF_BOOL call_hook(zval* hook)
{
    //触发可读事件闭包函数
    if(hook && container_instance.object)
    {
        //如果是闭包函数
        if(sff_check_zval_function(hook)) {
            //则触发函数并且传入变量
            zval
            return_result;
            //将接收到的数据打包传入
            zval
            args[1];//自定义参数
            zval
            receive_data;
            args[0] = *container_instance.object;
            call_user_function_ex(EG(function_table), NULL, hook,
                                  &return_result, 1, args, 0, NULL);
            if(Z_TYPE(return_result) == IS_FALSE)
            {
                return SFF_FALSE;
            }else{
                return SFF_TRUE;
            }
        }else{
            zend_error( E_ERROR, "connect hook is not callable");
        }
    }
    zend_error( E_ERROR, "connect hook is not registered");
    return  SFF_FALSE;
}

//关闭
int sff_close(int fd)
{
    if(close(fd) == 0) {
        call_hook(container_instance.close_hook);
    }else{
        return SFF_FALSE;
    }
    return SFF_TRUE;
}

//连接
int sff_socket_connect()
{
    int n,error;
    fd_set read_set,write_set;
    struct timeval tval;
    socklen_t  len;
    int client_fd = container_instance.socket_lib->sockfd;

    //设置套接字编程非阻塞
    container_instance.socket_lib->setnoblock(client_fd);

    error = 0;

    struct sockaddr_in client_struct;

    client_struct.sin_family = AF_INET;

    client_struct.sin_port = htons(container_instance.container_port);

    client_struct.sin_addr.s_addr = inet_addr(container_instance.container_ip);

    //对描述符进行链接
    int res = connect(client_fd,(struct sockaddr*)&client_struct,sizeof(client_struct));

    if(res < 0)
    {
        if(errno != EINPROGRESS)
            return SFF_FALSE;
    }

    if(res == 0)
    {
        //描述符返回错误码是0并且connect成功
        return call_hook(container_instance.connect_hook);
    }

    FD_ZERO(&read_set);

    FD_SET(client_fd,&read_set);

    //阻塞3秒
    write_set = read_set;
    tval.tv_sec = 3;
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

    if(error)
    {
        container_instance.socket_lib->close(client_fd);
        errno = error;
        return SFF_FALSE;
    }

    //触发成功的钩子
    return call_hook(container_instance.connect_hook);
}

//读取
ssize_t sff_socket_read(int sock_fd,const void *vptr,size_t n,__time_t timeout)
{
    size_t nleft;
    ssize_t nread;
    char* ptr;
    ptr = (char*)vptr;
    nleft = n;

    fd_set read_set;

    fd_set write_set;

    struct timeval tval;


    FD_ZERO(&read_set);

    FD_SET(container_instance.socket_lib->sockfd,&read_set);

    tval.tv_sec = timeout;
    tval.tv_usec = 0;

    int select_number;

    //套接字数目
    select_number = select(container_instance.socket_lib->sockfd+1,&read_set,&write_set,NULL,&tval);

    if(select_number > 0)
    {
        //如果说还有未读取的字节数，那么就应该继续读取

        if(FD_ISSET(sock_fd,&read_set)) {

            if ((nread = recv(sock_fd, ptr, nleft, 0)) < 0) {
                //如果说收到信号中断
                if (errno == EINTR) {
                    nread = 0;
                } else {
                    zend_error(E_WARNING, "recv error,errno:%d;error msg:%s;server ip:%s;port:%d", errno,
                               strerror(errno), container_instance.container_ip, container_instance.container_port);
                    return SFF_FALSE;
                }
            } else if (nread == 0) {
                //链接失败
                php_error_docref(NULL, E_WARNING, "read server %s:%d close", container_instance.container_ip,
                                 container_instance.container_port);

                //重新链接
                container_instance.socket_lib->reconnect();
                //断开了链接
                return SFF_TRUE;
            }

        }

    }

    return SFF_TRUE;
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
                continue;
            }else if(errno == EWOULDBLOCK){
                nwrite = 0;
                continue;
            }else if(errno == EPIPE){
                //执行重新链接
                container_instance.socket_lib->reconnect();
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
    /**
        * 由于是一个描述符，所以直接使用select做描述符监控了，这样就不再需要使用epoll了，因为毕竟描述符不多
        */
    fd_set read_set;

    fd_set write_set;

    struct timeval tval;

    socklen_t  len;

    int error;

    int n;

    int err_res;

    size_t read_size;

    char read_buf[READBUF];
    bzero(read_buf, sizeof(read_buf));

    SFF_BOOL connect_result;

    FD_ZERO(&read_set);

    FD_SET(container_instance.socket_lib->sockfd,&read_set);

    tval.tv_sec = 1;
    tval.tv_usec = 0;


    n = select(container_instance.socket_lib->sockfd+1,&read_set,&write_set,NULL,&tval);


    if(n > 0 && FD_ISSET(container_instance.socket_lib->sockfd,&read_set))
    {
        //如果说select大于0，那么有一种情况就是套接字可能已经被关闭了，但是服务端并不知道
        len = sizeof(error);


        //如果说发生了错误
        if((err_res = getsockopt(container_instance.socket_lib->sockfd,SOL_SOCKET,SO_ERROR,&error,&len))> 0)
        {
            if(error > 0) {
                zend_error(E_WARNING,"connect %s:%d error",container_instance.container_ip,container_instance.container_port);
                //这个套接字已经坏掉了,就进行重新的链接一直到成功为止
                if (errno != EINTR) {
                    container_instance.socket_lib->close(container_instance.socket_lib->sockfd);
                    connect_result = container_instance.socket_lib->connect();
                    while (connect_result != SFF_TRUE) {
                        connect_result = container_instance.socket_lib->connect();
                    }
                }
            }
        }else if(err_res == 0){
            //开始读取数据
            if((read_size = container_instance.socket_lib->read(container_instance.socket_lib->sockfd,&read_buf,sizeof(read_buf),1)) > 0)
            {
                //如果说读到了0个字节 说明已经断开链接了，这时候要做断线处理
                if(read_size == 0)
                {
                    //重新连接
                    container_instance.socket_lib->reconnect();
                    return SFF_FALSE;
                }
                //触发可读事件闭包函数
                if(container_instance.receive_data_hook)
                {
                    //如果是闭包函数
                    if(sff_check_zval_function(container_instance.receive_data_hook)) {
                        //则触发函数并且传入变量
                        zval
                        return_result;
                        //将接收到的数据打包传入
                        zval
                        args[1];//自定义参数
                        zval
                        receive_data;
                        zend_string * data_buf = zend_string_init(read_buf, strlen(read_buf), 0);
                        ZVAL_NEW_STR(&receive_data, data_buf);
                        args[0] = receive_data;
                        call_user_function_ex(EG(function_table), NULL, container_instance.receive_data_hook,
                                              &return_result, 1, args, 0, NULL);
                        //在这里释放掉字符串
                        zend_string_release(data_buf);
                    }
                }

            }

        }
    }

    //删除掉描述符的监控下次重新加入
    FD_CLR(container_instance.socket_lib->sockfd,&read_set);
    return SFF_TRUE;
}

