//
// Created by zhanglei on 19-3-22.
//
#ifndef SFF_SFF_PROCESS_H
#define SFF_SFF_PROCESS_H

#endif //SFF_SFF_PROCESS_H

#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"
#endif

//内存池管理
typedef struct _process_pool_manage
{
    void* mem;

    //申请一块内存快
    int (*mem_block_alloc)();

    //释放一块内存块
    int (*free)();

    //销毁整个内存池
    int (*destroy_pool)(struct _process_pool_manage* manage);

}process_pool_manage;

//内存块结构体
typedef struct _process_block{

    //上一个地址
    struct _process_block *prev;

    //下一个地址
    struct _process_block *next;

    //数据地址
    char data[0];

}process_block;

//内存池结构体
typedef struct _process_pool{

    //总内存大小
    size_t memory_size;

    //块大小
    uint32_t block_size;

    //块的数量
    uint32_t block_number;

    //头部地址
    process_block* head;

    //尾部地址
    process_block* tail;

}process_pool;


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

process_pool_manage* process_pool_manage_init(uint32_t block_size);

int process_pool_alloc();

int process_pool_free();

int process_pool_destroy(process_pool_manage* manage);


//初始化进程
SFF_BOOL sff_worker_init();

SFF_BOOL start_daemon();

SFF_BOOL spawn();

SFF_BOOL monitor();