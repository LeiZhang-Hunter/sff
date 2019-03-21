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


//Config的构造函数
PHP_METHOD (SffContainer, __construct);

//设置容器的配置选项
PHP_METHOD (SffContainer, setConfig);

//运行容器
PHP_METHOD (SffContainer, run);


PHP_METHOD (SffContainer, __destruct);

void factory_container_init();
