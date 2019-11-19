//
// Created by zhanglei on 19-3-19.
//
#ifndef SFF_SSF_COMMON_H

#include "../sff_common.h"

#endif

super_container container_instance;


CONTAINER_BOOL super_container_init() {

    if(container_instance.init_state == CONTAINER_RUNNING)
    {
        php_error_docref(NULL, E_ERROR, "Container has been loading");
        return CONTAINER_FALSE;
    }

    //初始化这个结构体
    bzero(&container_instance, sizeof(super_container));

    //初始化状态为1
    container_instance.init_state = CONTAINER_RUNNING;

    //初始化容器配置
    container_instance.set_container_config = set_container_config;

    //sff进程容器的地址
    container_instance.process_factory = emalloc(sizeof(sff_worker));

    //初始化工作者容器
    sff_worker_init();

    //初始化信号处理器的地址
    container_instance.signal_factory = emalloc(sizeof(sff_signal_handle));

    //初始化这个结构体
    init_signal_handle();

    //初始化容器的运行函数
    container_instance.run   = container_run;

    //销毁容器
    container_instance.destroy = destroy_container;

    //初始化socket的库
    container_instance.socket_lib = emalloc(sizeof(sff_socket_lib));

    //初始化
    init_socket_lib();

    //初始化日志库
    container_instance.log_lib = emalloc(sizeof(sff_log));

    init_log_lib();

    //获取进程pid
    container_instance.container_pid = getpid();

    bzero(&container_instance.container_guard,sizeof(struct flock));
    return CONTAINER_TRUE;
}




//设置容器的
CONTAINER_BOOL set_container_config(zend_string *config_key, zval *config_item) {
    //加载php配置中的配置
    if (strcmp(CONTAINER_CONFIG_USER, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance, user, config_item);

        //获取用户信息
        struct passwd* userinfo = getpwnam(container_instance.user);
        if(!userinfo)
        {
            php_error_docref(NULL, E_ERROR, "container user(%s) is not exist",container_instance.user);
        }

        //设置用户的uid
        setgid(userinfo->pw_gid);
        setuid(userinfo->pw_uid);
    }

    if (strcmp(CONTAINER_CONFIG_UMASK, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance, umask, config_item);
    }

    if (strcmp(CONTAINER_CONFIG_DIRECTORY, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance, directory, config_item);
    }

    if (strcmp(CONTAINER_CONFIG_LOGFILE, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance, logfile, config_item);
    }

    if (strcmp(CONTAINER_CONFIG_LOGFILEMAXBYTES, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(container_instance, logfile_maxbytes, config_item);
    }

    if (strcmp(CONTAINER_CONFIG_LOGFILEBACKUPS, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(container_instance, logfile_backups, config_item);
    }

    if (strcmp(CONTAINER_CONFIG_PIDFILE, ZSTR_VAL(config_key)) == 0) {
        //将pid写入文件当中
        if(Z_TYPE(*config_item) == IS_STRING) {
            SET_CONTAINER_CONFIG_STR(container_instance, pidfile, config_item);
        }else{
            php_error_docref(NULL, E_ERROR, "pid file must be string");
        }
    }

    if (strcmp(CONTAINER_CONFIG_CHILDLOGDIR, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance, childlogdir, config_item);
    }

    if (strcmp(CONTAINER_CONFIG_MINFDS, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(container_instance, minfds, config_item);
    }

    if (strcmp(CONTAINER_CONFIG_MINPROCS, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(container_instance, minprocs, config_item);
    }

    if (strcmp(CONTAINER_CONFIG_DAEMON, ZSTR_VAL(config_key)) == 0) {
        if (Z_TYPE(*config_item) == IS_TRUE) {
            container_instance.daemon = 1;
        } else {
            container_instance.daemon = 0;
        }
    }

    //查看是否需要进行远程链接如果不需要则不进行
    if (strcmp(CONTAINER_CONNECT_SERVER, ZSTR_VAL(config_key)) == 0) {
        if(Z_TYPE(*config_item) == IS_TRUE)
        {
            container_instance.connect_server = 1;
        }else{
            container_instance.connect_server = 0;
        }
    }

    //加入容器的地址
    if (strcmp(CONTAINER_IP, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance, container_ip, config_item);
    }

    //加入容器的端口
    if (strcmp(CONTAINER_PORT, ZSTR_VAL(config_key)) == 0) {
        zval
        new_item;
        ZVAL_COPY(&new_item, config_item);
        convert_to_long(&new_item);
        uint16_t port = (uint16_t)zval_get_long(&new_item);
        container_instance.container_port = port;
    }

    //设置socket的接收缓冲区大小
    if (strcmp(CONTAINER_RECV_BUF_SIZE, ZSTR_VAL(config_key)) == 0) {
        zval zval_recv_buf;
        ZVAL_COPY(&zval_recv_buf, config_item);
        convert_to_long(&zval_recv_buf);
        int recv_buf = (int)zval_get_long(&zval_recv_buf);
        container_instance.recv_buf = recv_buf;
    }

    //设置socket发送缓冲区的大小
    if (strcmp(CONTAINER_SEND_BUF_SIZE, ZSTR_VAL(config_key)) == 0) {
        zval zval_send_buf;
        ZVAL_COPY(&zval_send_buf, config_item);
        convert_to_long(&zval_send_buf);
        int send_buf = (uint16_t)zval_get_long(&zval_send_buf);
        container_instance.send_buf = send_buf;
    }

    //加入需要生产的进程
    if (strcmp(CONTAINER_PROCESS_POOL, ZSTR_VAL(config_key)) == 0) {

        //计算出要申请的数目
        int type = Z_TYPE(*config_item);
        if (Z_TYPE(*config_item) == IS_ARRAY) {
            uint32_t
            process_pool_count = (Z_ARRVAL_P(config_item)->nNumUsed);
            if (process_pool_count) {
                //初始化内存池
                container_instance.process_pool_manager = process_pool_manage_init(process_pool_count);

                //循环键值对写入需要操作的命令
                zend_string * process_config_key;
                zval * process_config_item;
                int process_index = 0;//进程池索引
                ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(config_item), process_config_key, process_config_item)
                        {
                            if (process_config_key == NULL) {
                                continue;
                            }

                            process_block *slice = process_pool_alloc();

                            //初始化进程名字
                            char *process_name = emalloc(strlen(ZSTR_VAL(process_config_key)));
                            bzero(process_name,strlen(ZSTR_VAL(process_config_key)));
                            strcpy(process_name, ZSTR_VAL(process_config_key));
                            slice->process_name = process_name;

                            //如果是数组
                            if (Z_TYPE(*process_config_item) == IS_ARRAY) {

                                HashTable * process_info = Z_ARRVAL_P(process_config_item);


                                //初始化启动命令
                                zval * start_cmd_info = zend_hash_str_find(process_info, "start", strlen("start"));
                                if ((start_cmd_info) && Z_TYPE(*start_cmd_info) == IS_STRING) {
                                    char *start_cmd_str = emalloc(strlen(Z_STRVAL(*start_cmd_info))+1);
                                    bzero(start_cmd_str,sizeof(Z_STRVAL(*start_cmd_info)));
                                    strcpy(start_cmd_str, Z_STRVAL(*start_cmd_info));
                                    slice->start_cmd = start_cmd_str;
                                }else{
                                    php_error_docref(NULL, E_ERROR, "%s start command must not be null",slice->process_name);
                                }

                                //初始化停止命令
                                zval * stop_cmd_info = zend_hash_str_find(process_info, "stop", strlen("stop"));
                                if ((stop_cmd_info) && Z_TYPE(*stop_cmd_info) == IS_STRING) {
                                    char *stop_cmd_str = emalloc(strlen(Z_STRVAL(*stop_cmd_info)) + 1);
                                    strcpy(stop_cmd_str, Z_STRVAL(*stop_cmd_info));
                                    slice->stop_cmd = stop_cmd_str;
                                }else{
                                    php_error_docref(NULL, E_ERROR, "%s stop command must not be null",slice->process_name);
                                }

                                //将进程的运行状态设置为0未运行
                                slice->state = NO_RUNING;
                                slice->index = process_index;
                                process_index++;
                            }

                        }ZEND_HASH_FOREACH_END();
            }else{
                //必须要加入进程到容器
                php_error_docref(NULL, E_ERROR, "please add process to container");
            }
        }

    }
}

//开始运行容器
CONTAINER_BOOL container_run() {

    //获取到内存池地址
    process_pool *pool = container_instance.process_pool_manager->mem;

    int process_count = 0;

    pid_t pid = 0;

    if(container_instance.connect_server == SFF_TRUE) {

        //初始化套接字
        container_instance.socket_lib->create();

        //链接远程服务器
        SFF_BOOL res = container_instance.socket_lib->connect();
        if (res == SFF_FALSE) {
            php_error_docref(NULL, E_ERROR, "connect %s:%d server error",container_instance.container_ip,container_instance.container_port);
            exit(-1);
        }
    }

//    container_instance.socket_lib->write(container_instance.socket_lib->sockfd,"hahahah",strlen("hahahah"));


    //循环池子创建进程
    if (pool->head) {
        process_block *start = pool->head;
        while (start) {

            if(start->state == RUNNING)
            {
                zend_error( E_WARNING, "process->pid:%d;process name:%s; process has been runing\n",start->pid,start->process_name);
                continue;
            }

            //堆池子进行循环然后开始生产
            pid = container_instance.process_factory->spawn(process_count);

            start->index = process_count;
            start->pid = pid;
            start->state = RUNNING;

            //进程启动后的回调函数
            start = start->next;

            //计数器循环+1
            process_count = process_count + 1;
        }
    }

    //如果说处于运行状态位
    while (container_instance.init_state) {
        if(container_instance.init_state == CONTAINER_RUNNING) {
            if (container_instance.connect_server == SFF_TRUE) {
                //开启循环
                container_instance.socket_lib->loop_work();
            }
            //开始打开监控
            container_instance.process_factory->monitor();
        }
    }
    return CONTAINER_TRUE;
}

//销毁容器
CONTAINER_BOOL destroy_container() {
    efree(container_instance.user);
    efree(container_instance.umask);
    efree(container_instance.directory);
    efree(container_instance.logfile);
    efree(container_instance.pidfile);
    efree(container_instance.childlogdir);
    efree(container_instance.process_factory);
    efree(container_instance.signal_factory);
    efree(container_instance.socket_lib);
    //销毁掉整个连接池
    if(container_instance.process_pool_manager) {
        container_instance.process_pool_manager->destroy_pool();
    }
    return CONTAINER_TRUE;
}


