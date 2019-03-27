//
// Created by zhanglei on 19-3-19.
//
#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"

#endif

super_container container_instance;


CONTAINER_BOOL super_container_init()
{

    //初始化这个结构体
    bzero(&container_instance,sizeof(super_container));

    container_instance.init_state = 1;

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

    container_instance.run = container_run;

    //销毁容器
    container_instance.destroy = destroy_container;


    return CONTAINER_TRUE;
}




//设置容器的
CONTAINER_BOOL set_container_config(zend_string *config_key,zval* config_item)
{
    //加载php配置中的配置
    if (strcmp(CONTAINER_CONFIG_USER, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance,user,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_UMASK, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance,umask,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_DIRECTORY, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance,directory,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_LOGFILE, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance,logfile,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_LOGFILEMAXBYTES, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(container_instance,logfile_maxbytes,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_LOGFILEBACKUPS, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(container_instance,logfile_backups,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_PIDFILE, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance,pidfile,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_CHILDLOGDIR, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance,childlogdir,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_MINFDS, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(container_instance,minfds,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_MINPROCS, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(container_instance,minprocs,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_DAEMON, ZSTR_VAL(config_key)) == 0) {
        if(Z_TYPE(*config_item)== IS_TRUE)
        {
            container_instance.daemon = 1;
        }else{
            container_instance.daemon = 0;
        }
    }

    //加入容器的地址
    if (strcmp(CONTAINER_IP, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(container_instance,container_ip,config_item);
    }

    //加入容器的端口
    if (strcmp(CONTAINER_PORT, ZSTR_VAL(config_key)) == 0) {
        if(Z_TYPE(*config_item) == IS_LONG)
        {
            zval new_item;
            ZVAL_COPY(&new_item,config_item);
            SET_CONTAINER_CONFIG_INT(container_instance,container_port,(&new_item));
        }else{
            php_printf("%d\n",Z_TYPE(*config_item));
        }
    }
//
    //加入需要生产的进程
    if (strcmp(CONTAINER_PROCESS_POOL, ZSTR_VAL(config_key)) == 0) {

        //计算出要申请的数目
        int type = Z_TYPE(*config_item);
        if(Z_TYPE(*config_item) == IS_ARRAY) {
            uint32_t process_pool_count = (Z_ARRVAL_P(config_item)->nNumUsed);
            if(process_pool_count)
            {
                //初始化内存池
                container_instance.process_pool_manager = process_pool_manage_init(process_pool_count);

                //循环键值对写入需要操作的命令
                zend_string *process_config_key;
                zval *process_config_item;
                ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(config_item), process_config_key, process_config_item) {
                            if (process_config_key == NULL) {
                                continue;
                            }

                            process_block* slice = process_pool_alloc();

                            //初始化进程名字
                            bzero(slice->process_name,sizeof(slice->process_name));

                            strcpy(slice->process_name,ZSTR_VAL(process_config_key));

                            //如果是数组
                            if(Z_TYPE(*process_config_item) == IS_ARRAY) {

                                HashTable* process_info = Z_ARRVAL_P(process_config_item);

                                //初始化启动命令
                                zval* start_cmd_info = zend_hash_str_find(process_info,"start",strlen("start"));
                                if((start_cmd_info) && Z_TYPE(*start_cmd_info) == IS_STRING)
                                {
//                                    php_printf("%s\n",Z_STRVAL(*start_cmd_info));
                                    bzero(slice->start_cmd, sizeof(slice->start_cmd));
                                    strcpy(slice->start_cmd, Z_STRVAL(*start_cmd_info));
                                }

                                //初始化停止命令
                                zval* stop_cmd_info = zend_hash_str_find(process_info,"stop",strlen("stop"));
                                if((stop_cmd_info) && Z_TYPE(*stop_cmd_info) == IS_STRING)
                                {
//                                    php_printf("%s\n",Z_STRVAL(*stop_cmd_info));
                                    bzero(slice->stop_cmd, sizeof(slice->stop_cmd));
                                    strcpy(slice->stop_cmd, Z_STRVAL(*stop_cmd_info));
                                }

                                //将进程的运行状态设置为0未运行
                                slice->state = NO_RUNING;
                            }

                        } ZEND_HASH_FOREACH_END();
            }
        }

    }
}

//开始运行容器
CONTAINER_BOOL container_run()
{

    return CONTAINER_TRUE;
}

CONTAINER_BOOL destroy_container()
{
    efree(container_instance.user);
    efree(container_instance.umask);
    efree(container_instance.directory);
    efree(container_instance.logfile);
    efree(container_instance.pidfile);
    efree(container_instance.childlogdir);
    efree(container_instance.process_factory);
    efree(container_instance.signal_factory);
    return CONTAINER_TRUE;
}


