//
// Created by zhanglei on 19-3-22.
//
#ifndef SFF_SFF_PROCESS_H
#define SFF_SFF_PROCESS_H

#endif //SFF_SFF_PROCESS_H

#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"
#endif

#define NO_RUNING 0
#define RUNNING 1

//内存块结构体
typedef struct _process_block{

    //上一个地址
    struct _process_block *prev;

    //下一个地址
    struct _process_block *next;

    pid_t pid;

    //运行状态
    uint16_t state;

    char *process_name;

    char *stop_cmd;

    char *start_cmd;

    //索引号
    uint16_t index;

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


    //正在使用的块数
    uint32_t block_use_num;

}process_pool;

//内存池管理
typedef struct _process_pool_manage
{
    process_pool* mem;

    //申请一块内存快
    process_block* (*mem_block_alloc)();

    //释放一块内存块
    int (*free)();

    //销毁整个内存池
    int (*destroy_pool)();



}process_pool_manage;




process_pool_manage* process_pool_manage_init(uint32_t block_size);

process_block* process_pool_alloc();

int process_pool_free();

int process_pool_destroy();

void process_pool_debug(process_pool_manage* manage);


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