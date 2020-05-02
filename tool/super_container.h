//
// Created by zhanglei on 19-3-19.
//

#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"
#endif

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

#define CONTAINER_RUNNING 1
#define CONTAINER_STOPPING 0

#define CONTAINER_CONFIG_USER   "user"
#define CONTAINER_CONFIG_UMASK  "umask"
#define CONTAINER_CONFIG_DIRECTORY  "directory"
#define CONTAINER_CONFIG_LOGFILE    "logfile"
#define CONTAINER_CONFIG_LOGFILEMAXBYTES    "logfile_maxbytes"
#define CONTAINER_CONFIG_LOGFILEBACKUPS "logfile_backups"
#define CONTAINER_CONFIG_CHILDLOGDIR    "childlogdir"
#define CONTAINER_CONFIG_MINFDS "minfds"
#define CONTAINER_CONFIG_MINPROCS   "minprocs"
#define CONTAINER_CONFIG_DAEMON   "daemon"
#define CONTAINER_CONNECT_SERVER   "connect_server"
#define CONTAINER_IP   "container_ip"
#define CONTAINER_PORT "container_port"
#define CONTAINER_PROCESS_POOL "process_pool"
#define CONTAINER_RECV_BUF_SIZE "container_recv_buff"
#define CONTAINER_SEND_BUF_SIZE "container_send_buff"
#define CONTAINER_CONFIG_PIDFILE    "pidfile"

//定义一个结构体
typedef struct{
    char *user;//container的运行用户
    char *umask;
    char *directory;
    char *logfile;
    size_t logfile_maxbytes;
    int logfile_backups;
    char* childlogdir;
    int minfds;
    int minprocs;

    //是否是守护进程
    int daemon;

    //初始化状态，这个结构体只可以被实例化一次除非释放掉
    int init_state;

    //监听的ip地址
    char *container_ip;


    zval* object;

    uint8_t connect_server;

    //监听的端口
    uint16_t container_port;

    CONTAINER_BOOL nocleanup;
    //打开php配置文件
    CONTAINER_BOOL (*set_container_config)(zend_string *config_key,zval* config_item);//设置容器的配置

    sff_worker *process_factory;

    process_pool_manage *process_pool_manager;

    sff_signal_handle *signal_factory;

    //运行容器
    CONTAINER_BOOL (*run)();//设置容器的配置

    //销毁容器
    CONTAINER_BOOL (*destroy)();//设置容器的配置

    //socket的链接库
    sff_socket_lib *socket_lib;

    //socket连接成功的钩子
    zval *connect_hook;

    zval *close_hook;

    //程序进程启动时候的钩子
    zval *process_start_hook;

    //程序进程关闭时候的钩子
    zval *process_stop_hook;

    //程序收到数据的时候的钩子
    zval *receive_data_hook;

    sff_log *log_lib;
    //pid的存储文件
    char *pidfile;

    pid_t container_pid;

    int recv_buf;

    int send_buf;

    //上锁标志
    struct flock container_guard;

    //循环这些次数以后就发送一个心跳包，防止客户端挂掉
    int loop_count;

}super_container;




#define SET_CONTAINER_CONFIG_STR(handle,key,item) \
if (Z_TYPE(*item) == IS_STRING){\
    char* user_name = emalloc(sizeof(char)*255);\
    bzero(user_name,sizeof(char)*255);\
    strcpy(user_name,Z_STRVAL(*item));\
    handle.key=user_name;\
}

#define SET_CONTAINER_CONFIG_INT(handle,key,item) \
if (Z_TYPE(*item) == IS_INDIRECT){\
    zend_long int_value;\
    memcpy(&int_value,&(item->value.lval),sizeof(int_value));\
    handle.key = int_value;\
}

//初始化super_container结构体
CONTAINER_BOOL super_container_init();


//设置容器的配置
CONTAINER_BOOL set_container_config(zend_string *config_key,zval* config_item);

//运行容器
CONTAINER_BOOL container_run();

//生成pid
CONTAINER_BOOL make_container_pid_file();

//销毁容器
CONTAINER_BOOL destroy_container();



