//
// Created by zhanglei on 19-3-22.
//
#include "sff_process.h"
SFF_BOOL sff_process_init(sff_worker* process_handle)
{

    process_handle->start_daemon = start_daemon;
    process_handle->monitor = monitor;
    process_handle->spawn = spawn;
    return SFF_TRUE;
}

SFF_BOOL start_daemon(sff_worker* process_handle)
{

    pid_t pid;

    int i;

    //创建进程失败
    if((pid = fork()) < 0) {
        return SFF_FALSE;
    }else if(pid)
    {
        //这是一个父进程杀死
        exit(0);
    }

    if(setsid() < 0)
        return SFF_FALSE;

    //忽略掉sighup这个信号

    //再次fork防止回话终端的控制权抢断
    if((pid = fork()) < 0) {
        return SFF_FALSE;
    }else if(pid)
    {
        //这是一个父进程杀死
        exit(0);
    }

    //更改代码运行的根目录
    chdir("/");

    //创造空闲的描述符
    for(i=0;i<process_handle->minfds;i++)
    {
        close(i);
    }

    //将读写执行定义到黑洞文件当中
    open("/dev/null",O_RDONLY);
    open("/dev/null",O_RDWR);
    open("/dev/null",O_RDWR);


    return SFF_TRUE;
}

//banner进程
SFF_BOOL spawn()
{
    pid_t pid;

    if((pid = fork()) < 0)
    {
        return SFF_FALSE;
    }else if(pid)
    {
        return pid;
    }
}

//执行程序监控
SFF_BOOL monitor()
{
    pid_t pid;
    int stat;
    pid = waitpid(1,&stat,WNOHANG);

    if(pid > 0)
    {
        //让进程退出的exit码
        WIFEXITED(stat);

        //另进程退出的信号
        WIFSIGNALED(stat);
    }
}