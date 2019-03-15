//
// Created by zhanglei on 19-2-20.
//

#ifndef SFF_SFF_APPLICATION_H
#define SFF_SFF_APPLICATION_H

#endif //SFF_SFF_APPLICATION_H
//在栈上申请内存
zend_class_entry ssf_application;

//栈上申请内存的实例 存储为一个全局变量
zend_class_entry *ssf_application_entry;

//定义参数
//定义run函数的参数
ZEND_BEGIN_ARG_INFO_EX(config_param_struct, 0, 0, 1)
                ZEND_ARG_INFO(0, root_path)
ZEND_END_ARG_INFO()

//定义请求到达之前的钩子函数
ZEND_BEGIN_ARG_INFO_EX(request_before_hook, 0, 0, 1)
                ZEND_ARG_INFO(0, before_hook)
ZEND_END_ARG_INFO()

//定义请求到达之后的钩子函数
ZEND_BEGIN_ARG_INFO_EX(request_after_hook, 0, 0, 1)
                ZEND_ARG_INFO(0, after_hook)
ZEND_END_ARG_INFO()

//定义设置配置选项的库函数
ZEND_BEGIN_ARG_INFO_EX(sff_config_struct, 0, 0, 1)
                ZEND_ARG_INFO(0, sff_config)
ZEND_END_ARG_INFO()

//主文件夹参数
ZEND_BEGIN_ARG_INFO_EX(sff_main_folder_struct, 0, 0, 1)
                ZEND_ARG_INFO(0, sff_config)
ZEND_END_ARG_INFO()

//定义接口
PHP_METHOD (SimSuperFactory, __construct);

//获取request的模块
PHP_METHOD (SimSuperFactory, getModule);

//获取request的控制器
PHP_METHOD (SimSuperFactory, getController);

//获取request的方法
PHP_METHOD (SimSuperFactory, getMethod);

//在请求到达前注入闭包函数
PHP_METHOD(SimSuperFactory,onRequestBefore);

//在请求到达后注入闭包函数
PHP_METHOD(SimSuperFactory,onRequestEnd);

//注入工厂配置
PHP_METHOD(SimSuperFactory,setConfig);

//获取注入的工厂配置
PHP_METHOD(SimSuperFactory,getConfig);

//设置工厂的主目录
PHP_METHOD(SimSuperFactory,setMain);

//运行超级工厂
PHP_METHOD (SimSuperFactory, run);

//工厂的析构函数
PHP_METHOD (SimSuperFactory, __destruct);

//定义所需要的属性宏

//默认模块
#ifndef SSF_DEFAULT_MODULE
#define SSF_DEFAULT_MODULE "default_module"
#endif

//默认控制器
#ifndef SSF_DEFAULT_CONTROLLER
#define SSF_DEFAULT_CONTROLLER "default_controller"
#endif

//默认控制器
#ifndef SSF_DEFAULT_METHOD
#define SSF_DEFAULT_METHOD "default_method"
#endif

//项目跟路径
#ifndef SFF_ROOT_DIR
#define SFF_ROOT_DIR "root_dir"
#endif

//run的前置钩子
#ifndef SFF_REQUEST_BEFORE_HOOK
#define SFF_REQUEST_BEFORE_HOOK "run_before_hook"
#endif

//run的后置钩子
#ifndef SFF_REQUEST_AFTER_HOOK
#define SFF_REQUEST_AFTER_HOOK "run_end_hook"
#endif

//定义项目自定义配置
#ifndef SFF_REQUEST_CONFIG
#define SFF_REQUEST_CONFIG "sff_config"
#endif



void ssf_application_init();