//
// Created by zhanglei on 19-3-19.
//
#include "../sff_common.h"



CONTAINER_BOOL super_container_init(super_container* handle)
{
    //初始化这个结构体
    bzero(handle,sizeof(super_container));

    //初始化容器配置
    handle->set_container_config = set_container_config;

    //销毁容器
    handle->destroy = destroy_container;
    return CONTAINER_TRUE;
}




//设置容器的
CONTAINER_BOOL set_container_config(super_container* handle,zend_string *config_key,zval* config_item)
{
    //加载php配置中的配置
    if (strcmp(CONTAINER_CONFIG_USER, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(handle,user,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_UMASK, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(handle,umask,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_DIRECTORY, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(handle,directory,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_LOGFILE, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(handle,logfile,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_LOGFILEMAXBYTES, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(handle,logfile_maxbytes,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_LOGFILEBACKUPS, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(handle,logfile_backups,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_PIDFILE, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(handle,pidfile,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_CHILDLOGDIR, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_STR(handle,childlogdir,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_MINFDS, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(handle,minfds,config_item);
    }

    if (strcmp(CONTAINER_CONFIG_MINPROCS, ZSTR_VAL(config_key)) == 0) {
        SET_CONTAINER_CONFIG_INT(handle,minprocs,config_item);
    }
}

CONTAINER_BOOL destroy_container(super_container* handle)
{
    return CONTAINER_TRUE;
}


