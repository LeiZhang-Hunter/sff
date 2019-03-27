//
// Created by zhanglei on 19-3-22.
//
#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"
#endif

SFF_BOOL sff_worker_init()
{
    if(!container_instance.process_factory)
    {
        return SFF_FALSE;
    }
    bzero(container_instance.process_factory,sizeof(sff_worker));
    container_instance.process_factory->start_daemon = start_daemon;
    container_instance.process_factory->monitor = monitor;
    container_instance.process_factory->spawn = spawn;
    return SFF_TRUE;
}

SFF_BOOL start_daemon()
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
    for(i=0;i<3;i++)
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

//初始化内存地址
process_pool_manage* process_pool_manage_init(uint32_t block_size)
{
    //计算内存池需要的字节数
    size_t pool_size = sizeof(process_pool_manage) + sizeof(process_pool) + block_size * sizeof(process_block);

    //初始化的起始地址
    void* mem = malloc(pool_size);

    //获取内存尺的首地址
    process_pool* pool = (process_pool*)((char*)mem + sizeof(process_pool_manage));

    //初始化地址内容
    bzero(pool,sizeof(process_pool));

    //块的大小
    pool->block_size = sizeof(process_block);

    //块的
    pool->block_number = block_size;

    //总共需要的内存
    pool->memory_size = block_size * sizeof(process_block);

    //初始化管理器的地址
    process_pool_manage* manage = mem;

    manage->mem = pool;

    manage->mem_block_alloc = process_pool_alloc;

    manage->free = process_pool_free;

    manage->destroy_pool = process_pool_destroy;

    return manage;
}

//申请这一块内存地址
process_block* process_pool_alloc()
{
    //内存池的地址
    process_pool* pool = container_instance.process_pool_manager->mem;

    //程序中没有可以用的内存块了
    if(pool->block_use_num >= pool->block_number)
    {
        return NULL;
    }

    //如果说头部指针的值为NULL,说明之前并没有申请过内存块
    if(pool->head == NULL)
    {
        //这里我们要初始化这个内存地址
        pool->head = (process_block*)((char *)pool+sizeof(process_pool));

        //第一次申请自然不会有下一个的指针
        pool->head->next = NULL;

        //初始化尾部的指针
        pool->tail = (process_block*)((char *)pool+sizeof(process_pool));

        //初始化内存块的地址
        bzero(pool->head,sizeof(process_block));

    }else{
        //旧的尾部块的地址
        process_block* old_tail_block = pool->tail;

        //现在的块地址
        process_block* this_block = (process_block*)((char*)old_tail_block+sizeof(process_block));

        //初始化这个块
        bzero(this_block,sizeof(process_block));

        //上一个块下一个地址变成可这一个块
        old_tail_block->next = this_block;

        //这一个块的上一个地址是上一个块
        this_block->prev = old_tail_block;

        //内存池的尾部地址变为最新块的地址
        pool->tail = this_block;

        //正在使用的内存池+1
        pool->block_use_num+=1;
    }

    return pool->tail;
}

int process_pool_free()
{

}

int process_pool_destroy(process_pool_manage* manage)
{
    free(manage);
}


//输出池子里所有块的索引
void process_pool_debug(process_pool_manage* manage)
{
    process_pool* pool = manage->mem;
    if(pool->head)
    {
        process_block* start = pool->head;
//        printf("%d\n",start->index);
        while(start)
        {
            php_printf("name:%s\n",start->process_name);
            php_printf("start:%s\n",start->start_cmd);
            php_printf("stop:%s\n",start->stop_cmd);
            start = start->next;
        }
    }
}


