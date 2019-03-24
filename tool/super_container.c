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

    //销毁容器
    container_instance.destroy = destroy_container;

    //sff进程容器的地址
    container_instance.process_factory = emalloc(sizeof(sff_worker));

    //初始化工作者容器
    sff_worker_init();
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
        //php_printf("%ld\n",config_item->value.lval);
        //php_printf("%d\n",222);
        if(Z_TYPE(*config_item)== IS_TRUE)
        {
            container_instance.daemon = 1;
        }else{
            container_instance.daemon = 0;
        }
    }
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
    return CONTAINER_TRUE;
}


