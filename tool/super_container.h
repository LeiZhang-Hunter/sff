//
// Created by zhanglei on 19-3-19.
//

#ifndef SFF_SUPER_CONTAINER_H
#define SFF_SUPER_CONTAINER_H

#endif //SFF_SUPER_CONTAINER_H

#define USERLEN 255

//
// Created by zhanglei on 19-3-19.
//

//定义一个全局的ini配置文件
PHP_INI_BEGIN()
        PHP_INI_ENTRY("sff.user", "sff", PHP_INI_ALL, NULL)
//                PHP_INI_ENTRY("sff.test_sff", "12222!", PHP_INI_ALL, NULL)
PHP_INI_END()

//定义bool的结构体
#define CONTAINER_BOOL int
#define CONTAINER_TRUE 0
#define CONTAINER_FALSE -1

#define CONTAINER_CONFIG_USER   "user"
#define CONTAINER_CONFIG_UMASK  "umask"
#define CONTAINER_CONFIG_DIRECTORY  "directory"
#define CONTAINER_CONFIG_LOGFILE    "logfile"
#define CONTAINER_CONFIG_LOGFILEMAXBYTES    "logfile_maxbytes"
#define CONTAINER_CONFIG_LOGFILEBACKUPS "logfile_backups"
#define CONTAINER_CONFIG_PIDFILE    "pidfile"
#define CONTAINER_CONFIG_CHILDLOGDIR    "childlogdir"
#define CONTAINER_CONFIG_MINFDS "minfds"
#define CONTAINER_CONFIG_MINPROCS   "minprocs"

//定义一个结构体
typedef struct _sff_container{
    char *user;//container的运行用户
    char *umask;
    char *directory;
    char *logfile;
    size_t logfile_maxbytes;
    int logfile_backups;
    char* pidfile;
    char* childlogdir;
    int minfds;
    int minprocs;
    
    CONTAINER_BOOL nocleanup;
    //打开php配置文件
    CONTAINER_BOOL (*set_container_config)(struct _sff_container* handle,zend_string *config_key,zval* config_item);//设置容器的配置

    //销毁容器
    CONTAINER_BOOL (*destroy)(struct _sff_container* handle);//设置容器的配置

}super_container;

#define SET_CONTAINER_CONFIG_STR(handle,key,item) \
if (Z_TYPE(*item) == IS_STRING){\
    char* user_name = emalloc(sizeof(char)*255);\
    bzero(user_name,sizeof(char)*255);\
    strcpy(user_name,Z_STRVAL(*item));\
    handle->key=user_name;\
}

#define SET_CONTAINER_CONFIG_INT(handle,key,item) \
if (Z_TYPE(*item) == IS_INDIRECT){\
    handle->key = item->value.lval;\
}

//初始化super_container结构体
CONTAINER_BOOL super_container_init(super_container* handle);


//设置容器的配置
CONTAINER_BOOL set_container_config(super_container* handle,zend_string *config_key,zval* config_item);

//销毁容器
CONTAINER_BOOL destroy_container(super_container* handle);




