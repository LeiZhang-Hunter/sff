//
// Created by zhanglei on 19-3-22.
//

#include <ext/sff/sff_common.h>

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

    //重启的指令
    char* start_cmd;

    //关闭的指令
    char* stop_cmd;

    //重载
    char* reload_cmd;



}worker_process;

//进程池
typedef struct _worker_process_pool
{
    //内存池尺度
    size_t init_pool_size;
    //块大小
    size_t block_size;
    //块数量
    size_t block_number;
}worker_process_pool;

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

}sff_worker;

//初始化进程
SFF_BOOL sff_worker_init();

SFF_BOOL start_daemon();

SFF_BOOL spawn();

SFF_BOOL monitor();