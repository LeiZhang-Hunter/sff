//
// Created by root on 18-11-13.
//

#ifndef SFF_SSF_COMMON_H
#define SFF_SSF_COMMON_H

#endif //SFF_SSF_COMMON_H

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_sff.h"
#include "php_main.h"
#ifndef	_FCNTL_H
#include <fcntl.h>
#endif

#ifndef	_SYS_WAIT_H
#include <sys/wait.h>
#endif


#ifndef	_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifndef _ARPA_INET_H
#include <arpa/inet.h>
#endif


//引入php内核异常处理
#ifndef ZEND_EXCEPTIONS_H
#include <zend_exceptions.h>
#endif

#include <sys/prctl.h>


#ifndef SFF_LOAD_FILE_H
#include "tool/load_file.h"

#endif //SFF_LOAD_FILE_H

//载入ssf 中 处理字符串的头文件
#ifndef SFF_SFF_STRING_H
#include "tool/ssf_string.h"
#endif //SFF_SFF_STRING_H

//载入ssf中zend的接口
#ifndef SFF_INTERFACE_H
#include "zend_ssf_interface/interface.h"
#endif

//载入定义的zend全局变量
#ifndef SFF_SSF_GLOBAL_H
#include "factory/sff_global.h"
#endif

//载入配置类库的头文件
#ifndef SFF_LOAD_CONFIG_H
#include "sff_interface/load_config.h"
#endif

//加载路由函数接口
#ifndef SFF_SFF_ROUTE_H
#include "tool/ssf_route.h"
#endif

//加载主类的接口
#ifndef SFF_SFF_APPLICATION_H
#include "sff_interface/ssf_application.h"
#endif

//加载工厂控制器
#ifndef SFF_SUPER_FACTORY_H
#include "sff_interface/factory_controller.h"
#endif

//加载自动加载的控制器
#ifndef SFF_SFF_LOADER_H
#include "sff_interface/sff_loader.h"
#endif

//加载闭包函数的库
#ifndef ZEND_CLOSURES_H
#include <zend_closures.h>
#endif

//加载信号处理池
#ifndef SFF_SFF_CONTAINER_H
#include "tool/sff_signal_handle.h"
#endif




#ifndef SFF_SFF_PROCESS_H
#include "tool/sff_process.h"
#endif

#ifndef SFF_SFF_SOCKET_H
#include "tool/sff_socket.h"
#endif

#ifndef SFF_SUPER_CONTAINER_H
#include "tool/log_file.h"
#endif //SFF_SFF_CONTAINER_H


#ifndef SFF_SUPER_CONTAINER_H
#include "tool/super_container.h"
#endif //SFF_SFF_CONTAINER_H

extern super_container container_instance;

//加载闭包函数的库
#ifndef SFF_SFF_CONTAINER_H
#include "sff_interface/sff_container.h"
#endif

#ifndef SFF_SFF_PDO_POOL_H
#include "sff_interface/sff_pdo_pool.h"
#endif



