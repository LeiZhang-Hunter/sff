//
// Created by root on 18-11-15.
//

#ifndef SFF_LOAD_CONFIG_H
#define SFF_LOAD_CONFIG_H

#endif //SFF_LOAD_CONFIG_H


//
zend_class_entry config;
zend_class_entry *config_ce;


//定义参数
ZEND_BEGIN_ARG_INFO_EX(config_option_struct, 0, 0, 1)
                ZEND_ARG_INFO(0, single_option)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(config_path_struct, 0, 0, 1)
                ZEND_ARG_INFO(0, config_path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(config_before_function_struct, 0, 0, 1)
                ZEND_ARG_INFO(0, config_before_function)
ZEND_END_ARG_INFO()

//Config的构造函数
PHP_METHOD (Config, __construct);

//获取Config的实例
PHP_METHOD (Config, getInstance);

//初始化配置
PHP_METHOD (Config, initConfig);

//注册在获取之前的操作处理
PHP_METHOD(Config,regInitBeforeStep);

//注册在获取配置之后的操作处理
PHP_METHOD(Config,regInitAfterStep);

//获取配置文件
PHP_METHOD(Config,getConfig);

//析构函数
PHP_METHOD (Config, __destruct);


//定义需要的属性宏
//获取配置前的钩子
#ifndef SFF_BEFORE_HOOK
#define SFF_BEFORE_HOOK "config_before_hook"
#endif

//获取配置后的钩子
#ifndef SFF_FINISH_HOOK
#define SFF_FINISH_HOOK "config_finish_hook"
#endif

//获取配置文件
#ifndef SFF_CONFIG_DATA
#define SFF_CONFIG_DATA "config_data"
#endif

//初始化Config类
void ssf_config_init();