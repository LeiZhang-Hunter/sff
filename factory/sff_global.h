//
// Created by zhanglei on 19-2-19.
//

#ifndef SFF_SSF_GLOBAL_H
#define SFF_SSF_GLOBAL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#endif //SFF_SFF_GLOBAL_H


//定义自己要用的全局变量
ZEND_BEGIN_MODULE_GLOBALS(sff)
    //用来检查类包是否重复运行，防止重复运行
    zend_bool run_status;

    //模块
    zend_string *current_module;

    //控制器
    zend_string *current_controller;

    //方法
    zend_string *current_method;

    //根目录网站的
    zend_string *root_dir;

    //项目的主目录
    char *main_folder;

    //项目的主配置项
    zval *config;

ZEND_END_MODULE_GLOBALS(sff)

//重新定义全局变量结构体
ZEND_DECLARE_MODULE_GLOBALS(sff)

//赋值和获取全局变量的宏
#define SFF_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(sff, v)