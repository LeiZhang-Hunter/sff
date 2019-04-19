//
// Created by zhanglei on 19-3-15.
//

#ifndef SFF_SFF_CONTAINER_H
#define SFF_SFF_CONTAINER_H

#endif //SFF_SFF_CONTAINER_H

//在栈上申请内存
zend_class_entry factory_container;

//栈上申请内存的实例 存储为一个全局变量
zend_class_entry *factory_container_entry;

//定义容器的配置
ZEND_BEGIN_ARG_INFO_EX(container_config_struct, 0, 0, 2)
                ZEND_ARG_INFO(0, config_value)
ZEND_END_ARG_INFO()

//注册进程启动时候的钩子
ZEND_BEGIN_ARG_INFO_EX(process_start_hook, 0, 0, 1)
                ZEND_ARG_INFO(0, config_value)
ZEND_END_ARG_INFO()

//注册connect的钩子
ZEND_BEGIN_ARG_INFO_EX(container_connect_hook, 0, 0, 1)
                ZEND_ARG_INFO(0, config_value)
ZEND_END_ARG_INFO()

//注册关闭的钩子
ZEND_BEGIN_ARG_INFO_EX(container_close_hook, 0, 0, 1)
                ZEND_ARG_INFO(0, config_value)
ZEND_END_ARG_INFO()

//注册收到数据的钩子
ZEND_BEGIN_ARG_INFO_EX(recieve_data_hook, 0, 0, 1)
                ZEND_ARG_INFO(0, config_value)
ZEND_END_ARG_INFO()

//注册进程结束的钩子
ZEND_BEGIN_ARG_INFO_EX(process_stop_hook, 0, 0, 1)
                ZEND_ARG_INFO(0, config_value)
ZEND_END_ARG_INFO()

//要发送的数据
ZEND_BEGIN_ARG_INFO_EX(send_data, 0, 0, 1)
                ZEND_ARG_INFO(0, config_value)
ZEND_END_ARG_INFO()

//停止程序传入的停止索引
ZEND_BEGIN_ARG_INFO_EX(stop_index, 0, 0, 1)
                ZEND_ARG_INFO(0, config_value)
ZEND_END_ARG_INFO()

//启动程序的启动索引
ZEND_BEGIN_ARG_INFO_EX(start_index, 0, 0, 1)
                ZEND_ARG_INFO(0, config_value)
ZEND_END_ARG_INFO()


//Config的构造函数
PHP_METHOD (SffContainer, __construct);

//设置容器的配置选项
PHP_METHOD (SffContainer, setConfig);

//链接到云端的钩子
PHP_METHOD (SffContainer, socketConnectHook);

//关闭套接字的钩子
PHP_METHOD (SffContainer, socketCloseHook);

//注册接收钩子
PHP_METHOD (SffContainer, receiveHook);

//注册进程启动的钩子
PHP_METHOD (SffContainer, processStartHook);

//注册进程退出的钩子
PHP_METHOD (SffContainer, processStopHook);

//容器上报
PHP_METHOD (SffContainer,report);

//停止进程
PHP_METHOD (SffContainer, stop);

//再一次启动进程
PHP_METHOD (SffContainer, start);

//运行容器
PHP_METHOD (SffContainer, run);


PHP_METHOD (SffContainer, __destruct);

#define CONTAINER_CONNECT_HOOK "container_connect_hook"

#define CONTAINER_CLOSE_HOOK "container_close_hook"

#define CONTAINER_RECV_HOOK "receive_data_hook"

#define CONTAINER_RROC_START_HOOK "process_stop_hook"

#define CONTAINER_RROC_STOP_HOOK "process_start_hook"

void factory_container_init();
