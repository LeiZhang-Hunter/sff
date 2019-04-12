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
#define KILLED 2
#define STOPPED 3

//进程名字
#define SFF_PROCESS_NAME "process_name"

//进程pid
#define SFF_PID "pid"

//linux 信号
#define SFF_PROC_SIG_NO "sig_no"

//退出的错误码
#define SFF_PROC_EXIT_CODE "exit_code"

//运行状态
#define SFF_PROC_RUN_STATE "run_state"

//进程池索引
#define SFF_PROC_INDEX "index"

//启动指令
#define SFF_PROC_START_CMD "start_cmd"

//退出指令
#define SFF_PROC_STOP_CMD "stop_cmd"

//内存块结构体
typedef struct _process_block{

    //上一个地址
    struct _process_block *prev;

    //下一个地址
    struct _process_block *next;

    pid_t pid;

    //导致程序退出的linux信号码
    int sig_no;

    //退出的错误码
    int exit_code;

    //运行状态
    uint16_t state;

    char *process_name;

    char *stop_cmd;

    char *start_cmd;

    //索引号
    int index;

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

    //根据pid来获取进程块
    process_block* (*get_block_by_pid)(pid_t pid);

    //释放一块内存块
    int (*free)();

    //销毁整个内存池
    int (*destroy_pool)();

    int (*send_message)(int signo);

}process_pool_manage;


process_block* get_block_by_pid(pid_t pid);

process_pool_manage* process_pool_manage_init(uint32_t block_size);

process_block* process_pool_alloc();

int process_pool_free();

int process_pool_destroy();

void process_pool_debug(process_pool_manage* manage);


typedef struct _sff_worker{

    //创建一个守护进程
    SFF_BOOL (*start_daemon)();

    //创建一个进程
    SFF_BOOL (*spawn)(pid_t process_count);

    //监控进程
    int (*monitor)();

    //最小的进程数目
    int minfds;

    //进程启动的钩子
    void (*start_hook)(process_block* process_info);

    void (*stop_hook)(process_block* process_info);
}sff_worker;

//初始化进程
SFF_BOOL sff_worker_init();

SFF_BOOL start_daemon();

/**
 * 生产进程
 * @param process_count 这个是进程的index，用于做偏移
 * @return
 */
SFF_BOOL spawn(pid_t process_count);

//进程启动后的钩子
void process_start_call_hook(process_block* process_info);

//进程停止后的钩子
void process_stop_call_hook(process_block* process_info);

SFF_BOOL monitor();

//堆内存池中所有的进程发送信号
int send_message_pool(int signo);