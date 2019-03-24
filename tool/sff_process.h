//
// Created by zhanglei on 19-3-22.
//

#ifndef SFF_SFF_PROCESS_H
#define SFF_SFF_PROCESS_H

#endif //SFF_SFF_PROCESS_H

#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"
#endif



typedef struct _worker_process{
    //进程号
    pid_t pid;

    //错误码
    int worker_errno;

    //运行状态
    int state;
}worker_process;

typedef struct _sff_worker{

    //创建一个守护进程
    SFF_BOOL (*start_daemon)();

    //创建一个进程
    SFF_BOOL (*spawn)();

    //监控进程
    int (*monitor)();

    //最小的进程数目
    int minfds;

    //进程池
    worker_process** process_collect;

}sff_worker;

//初始化进程
SFF_BOOL sff_worker_init();

SFF_BOOL start_daemon();

SFF_BOOL spawn();

SFF_BOOL monitor();