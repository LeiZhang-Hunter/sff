//
// Created by zhanglei on 19-3-22.
//
#ifndef SFF_SSF_COMMON_H

#include "../sff_common.h"

#endif

//初始化工作进程
SFF_BOOL sff_worker_init() {
    if (!container_instance.process_factory) {
        return SFF_FALSE;
    }
    bzero(container_instance.process_factory, sizeof(sff_worker));
    container_instance.process_factory->start_daemon = start_daemon;
    container_instance.process_factory->monitor = monitor;
    container_instance.process_factory->spawn = spawn;
    container_instance.process_factory->start_hook = process_start_call_hook;
    container_instance.process_factory->stop_hook = process_stop_call_hook;
    container_instance.process_factory->exec = process_exec;
    return SFF_TRUE;
}

SFF_BOOL start_daemon() {

    pid_t pid;

    int i;

    //创建进程失败
    if ((pid = fork()) < 0) {
        return SFF_FALSE;
    } else if (pid) {
        //这是一个父进程杀死
        exit(0);
    }

    if (setsid() < 0)
        return SFF_FALSE;

    //忽略掉sighup这个信号

    //再次fork防止回话终端的控制权抢断
    if ((pid = fork()) < 0) {
        return SFF_FALSE;
    } else if (pid) {
        //这是一个父进程杀死
        exit(0);
    }

    //更改代码运行的根目录
    chdir("/");

    //创造空闲的描述符
    for (i = 0; i < 3; i++) {
        close(i);
    }

    //将读写执行定义到黑洞文件当中
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);


    return SFF_TRUE;
}

//banner进程
SFF_BOOL spawn(pid_t process_count) {
    pid_t pid;
    char *arg;
    //偏移指针，找到对应的程序块
    process_block *block = container_instance.process_pool_manager->mem->head + process_count;
    //动态传递参数
    char *argv[0];

    if ((pid = vfork()) < 0) {
        return SFF_FALSE;
    } else if (pid) {
        block->state = RUNNING;
        //重置信息
        block->pid = pid;
        block->sig_no = 0;
        block->exit_code = 0;
        //触发启动的回调函数(不要触发下面有触发)
//        container_instance.process_factory->start_hook(block);
        return pid;
    }

#ifdef __linux__
    if(prctl(PR_SET_PDEATHSIG, SIGTERM) != 0)
    {
        zend_error(E_WARNING,"prctl error,errno:%d;msg:%s",errno,strerror(errno));
    }
#endif

    block->state = RUNNING;

    int res = container_instance.process_factory->exec(block->start_cmd);
    if(res == -1)
    {
        php_error_docref(NULL, E_WARNING, "%s start failed",block->process_name);
    }
    //程序运行正常结束
    _exit(0);

    return CONTAINER_TRUE;
}

SFF_BOOL process_exec(char* command)
{
    if(command == NULL)
    {
        return SFF_FALSE;
    }
    char str[strlen(command) + 1];

    strcpy(str, command);

    //解析参数地址
    char *exec_script = strtok(str, " ");

    //动态传递参数
    char *argv[0];

    char *arg;

    //计算总数
    int count = 0;
    argv[count] = "-f";
    while ((arg = strtok(NULL, " "))) {
        count = count + 1;
        argv[count] = arg;
    }
    argv[count + 1] = NULL;

    int res = execvp(exec_script, argv);

    return res;
}

//执行程序监控
SFF_BOOL monitor() {
    pid_t pid;
    int stat;

    pid = waitpid(-1, &stat, WNOHANG);



    if (pid > 0) {


        //从进程池里获取到对应的进程块
        process_block *block = container_instance.process_pool_manager->get_block_by_pid(pid);

        if (block) {

            //如果说进程已经停止了要删除进程的pid文件这样才能重启脚本否则造成脚本无法重启

            //让进程退出的exit码
            block->exit_code = WIFEXITED(stat);

            //另进程退出的信号
            block->sig_no = WIFSIGNALED(stat);

            //如果进程是正常的状态位那么说明是异常终止
            if (block->state == RUNNING) {

                //进程状态变为被杀死
                block->state = KILLED;

                //触发停止的钩子
                container_instance.process_factory->stop_hook(block);

                //删除对应的pid文件防止启动不起来
                if(block->pid_file) {
                    unlink(block->pid_file);
                }

                //重新拉起这个进程
                pid = container_instance.process_factory->spawn(block->index);

            } else if (block->state == STOPPED) {
                //就此停止这个进程,不做任何重启处理直接回收即可
                container_instance.process_factory->stop_hook(block);
            }
        }


    }

}

//初始化内存地址
process_pool_manage *process_pool_manage_init(uint32_t block_size) {
    //计算内存池需要的字节数
    size_t
    pool_size = sizeof(process_pool_manage) + sizeof(process_pool) + block_size * sizeof(process_block);

    //初始化的起始地址
    void *mem = emalloc(pool_size);

    //获取内存尺的首地址
    process_pool *pool = (process_pool *) ((char *) mem + sizeof(process_pool_manage));

    //初始化地址内容
    bzero(pool, sizeof(process_pool));

    //块的大小
    pool->block_size = sizeof(process_block);

    //块的
    pool->block_number = block_size;

    //总共需要的内存
    pool->memory_size = block_size * sizeof(process_block);

    //初始化管理器的地址
    process_pool_manage *manage = mem;

    manage->mem = pool;

    manage->mem_block_alloc = process_pool_alloc;

    manage->free = process_pool_free;

    manage->destroy_pool = process_pool_destroy;

    manage->get_block_by_pid = get_block_by_pid;

    manage->send_message = send_message_pool;

    return manage;
}

/**
 * 根据pid来获取进程块
 * @param pid
 * @return
 */
process_block *get_block_by_pid(pid_t pid) {
    process_pool *pool = container_instance.process_pool_manager->mem;
    if (pool->head) {
        process_block *start = pool->head;
//        printf("%d\n",start->index);
        while (start) {
            if (start->pid == pid) {
                return start;
            }
            start = start->next;
        }
    }
    return NULL;
}

//申请这一块内存地址
process_block *process_pool_alloc() {
    //内存池的地址
    process_pool *pool = container_instance.process_pool_manager->mem;

    //程序中没有可以用的内存块了
    if (pool->block_use_num >= pool->block_number) {
        return NULL;
    }

    //如果说头部指针的值为NULL,说明之前并没有申请过内存块
    if (pool->head == NULL) {
        //这里我们要初始化这个内存地址
        pool->head = (process_block *) ((char *) pool + sizeof(process_pool));

        //第一次申请自然不会有下一个的指针
        pool->head->next = NULL;

        //初始化尾部的指针
        pool->tail = (process_block *) ((char *) pool + sizeof(process_pool));

        //初始化内存块的地址
        bzero(pool->head, sizeof(process_block));

    } else {
        //旧的尾部块的地址
        process_block *old_tail_block = pool->tail;

        //现在的块地址
        process_block *this_block = (process_block *) ((char *) old_tail_block + sizeof(process_block));

        //初始化这个块
        bzero(this_block, sizeof(process_block));

        //上一个块下一个地址变成可这一个块
        old_tail_block->next = this_block;

        //这一个块的上一个地址是上一个块
        this_block->prev = old_tail_block;

        //内存池的尾部地址变为最新块的地址
        pool->tail = this_block;

        //正在使用的内存池+1
        pool->block_use_num += 1;
    }

    return pool->tail;
}

int process_pool_free() {

}

int process_pool_destroy() {
    //依次清除掉所有的命令
    process_pool *pool = container_instance.process_pool_manager->mem;
    if (pool->head) {
        process_block *start = pool->head;
        while (start) {
            efree(start->process_name);
            efree(start->start_cmd);
            efree(start->stop_cmd);
            start = start->next;
        }
    }
    //销毁掉整个连接池
    efree(container_instance.process_pool_manager);

    return SFF_TRUE;
}


//输出池子里所有块的索引
void process_pool_debug(process_pool_manage *manage) {
    process_pool *pool = manage->mem;
    if (pool->head) {
        process_block *start = pool->head;
//        printf("%d\n",start->index);
        while (start) {
            start = start->next;
        }
    }
}

//给进程池里的所有进程发送信号
int send_message_pool(int signo)
{
//依次清除掉所有的命令
    process_pool *pool = container_instance.process_pool_manager->mem;
    if (pool->head) {
        process_block *start = pool->head;
        while (start) {
            if(kill(start->pid,signo) < 0)
            {
                if(signo == SIGTERM) {
                    //将进程的状态位置设置为关闭防止再次被拉起来
                    start->state = STOPPED;
                }
                //标记发送失败
                php_error_docref(NULL, E_WARNING, "send signo failed,message:%s",strerror(errno));
            }
            start = start->next;
        }
    }
}

//启动的钩子
void process_start_call_hook(process_block* process_info)
{
    //触发可读事件闭包函数
    if(container_instance.process_start_hook)
    {
        //如果是闭包函数
        if(sff_check_zval_function(container_instance.process_start_hook)) {
            //则触发函数并且传入变量
            zval return_result;
            //将接收到的数据打包传入
            zval args[1];//自定义参数
            zval process_array;
            convert_process_block_zval_array(process_info,&process_array);

            args[0] = process_array;
            call_user_function_ex(EG(function_table), NULL, container_instance.process_start_hook,&return_result, 1, args, 0, NULL);
            //触发完成回调函数清除字符串缓冲
        }
    }
}

//关闭的钩子
void process_stop_call_hook(process_block* process_info)
{
    //触发可读事件闭包函数
    if(container_instance.process_stop_hook)
    {
        //如果是闭包函数
        if(sff_check_zval_function(container_instance.process_stop_hook)) {
            //则触发函数并且传入变量
            zval return_result;
            //将接收到的数据打包传入
            zval args[1];//自定义参数
            zval process_array;
            convert_process_block_zval_array(process_info,&process_array);

            args[0] = process_array;
            call_user_function_ex(EG(function_table), NULL, container_instance.process_stop_hook,&return_result, 1, args, 0, NULL);
            //触发完成回调函数清除字符串缓冲
        }
    }
}

