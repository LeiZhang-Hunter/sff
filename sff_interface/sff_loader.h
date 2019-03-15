//
// Created by zhanglei on 19-2-23.
//

#ifndef SFF_SFF_LOADER_H
#define SFF_SFF_LOADER_H

#endif //SFF_SFF_LOADER_H

//在栈上申请内存
zend_class_entry sff_loader;

//栈上申请内存的实例 存储为一个全局变量
zend_class_entry *sff_loader_entry;

//定义添加命名空间的参数
ZEND_BEGIN_ARG_INFO_EX(namespace_struct, 0, 0, 2)
                ZEND_ARG_INFO(0, namespace_key)
                ZEND_ARG_INFO(0, namespace_value)
ZEND_END_ARG_INFO()

//添加监听的闭包函数
//ZEND_BEGIN_ARG_INFO_EX(namespace_monitor_struct, 0, 0, 1)
//                ZEND_ARG_INFO(0, monitor_function)
//ZEND_END_ARG_INFO()

//添加启动函数
ZEND_BEGIN_ARG_INFO_EX(namespace_store_struct, 0, 0, 1)
                ZEND_ARG_INFO(0, monitor_function)
ZEND_END_ARG_INFO()

//记载类的构造函数
PHP_METHOD (SuperAutoLoader, __construct);

//添加命名空间
PHP_METHOD(SuperAutoLoader,addNameSpace);

//监听
PHP_METHOD(SuperAutoLoader,start_monitoring);

//工厂的析构函数
PHP_METHOD (SuperAutoLoader, __destruct);

//自动加载函数
PHP_METHOD(SuperAutoLoader,auto_monitoring);

//定义自己的静态实例
#ifndef SFF_NAMESPANCE
#define SFF_NAMESPANCE "namespace_store"
#endif

#ifndef SFF_SPL_AUTO_LOAD_FUNCTION
#define SFF_SPL_AUTO_LOAD_FUNCTION "spl_autoload_register"
#endif

#ifndef SFF_LOADER_CLASS
#define SFF_LOADER_CLASS "SuperAutoLoader"
#endif

#ifndef SFF_AUTO_LOAD_FUNCTION
#define SFF_AUTO_LOAD_FUNCTION "auto_monitoring"
#endif

void sff_load_init();