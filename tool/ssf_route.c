//
// Created by root on 18-12-3.
//
#include "../sff_common.h"


//初始化路由的句柄
sff_route_pack* ssf_route_pack_init(sff_route_pack *handel)
{
    handel->dispatch = sff_route_dispatch;
    handel->run = sff_route_run;
    handel->set_root_dir = sff_set_root_dir;
    handel->destroy = destroy;
    return handel;
}

//对路由的模块、控制器以及函数方法进行拆分
SFF_BOOL sff_route_dispatch(struct sff_route_pack_ *handel)
{
    char *url;
//    char *url = "/Pure/Device/addFan?uuid=EA6C803C0F6FAD967A2633FD28CC0AC6&EN=0";

    zval *server,*uri = NULL;

    //在栈上申请一个新的内存,重新存储url
    char url_buff[2048];
    bzero(url_buff,sizeof(url_buff));
//    strcpy(url_buff,url);

    if (PG(auto_globals_jit)) {
        zend_string *server_str = zend_string_init(ZEND_STRL("_SERVER"), 0);
        zend_is_auto_global(server_str);
        zend_string_release(server_str);
    }

    //获取$_SERVER变量
    server = &PG(http_globals)[TRACK_VARS_SERVER];

    //如果说对象是数组
    if (Z_TYPE((*server)) == IS_ARRAY){
        //发现$_SERVER中的REQUEST_URI
        uri = zend_hash_str_find(Z_ARRVAL(PG(http_globals)[TRACK_VARS_SERVER]), "REQUEST_URI", strlen("REQUEST_URI"));
        if(uri) {
            if (Z_TYPE(*(uri)) == IS_STRING) {
                url = ZSTR_VAL(Z_STR(*uri));
                strcpy(url_buff, url);
            } else {
                php_error_docref(NULL, E_ERROR, "$_SERVER REQUEST_URI Error");
            }
        }else{
            php_error_docref(NULL, E_ERROR, "$_SERVER REQUEST_URI Error");
        }
    }

    if(url == NULL)
    {
        php_error_docref(NULL, E_ERROR, "get url Error");
    }

    char *request_uri;

    //模块
    char *module;

    //在栈上申请模块
    char module_buf[MAXBUF];

    //存储到结构体的模块地址
    char *save_module;

    //控制器
    char *controller;

    //在栈上申请控制器
    char controller_buf[MAXBUF];

    //存储到结构体的控制器地址
    char *save_controller;

    //方法
    char *method;

    //存储到结构体的方法地址
    char *save_method;

    //在栈上申请方法
    char method_buf[MAXBUF];

    //格式化栈上的路由信息
    bzero(module_buf,sizeof(module_buf));
    bzero(controller_buf, sizeof(controller_buf));
    bzero(method_buf, sizeof(method_buf));

    //在堆上申请空间,并且将内容初始化
    save_module = (char*)emalloc(NAME_MAX);
    bzero(save_module,NAME_MAX);
    save_controller = (char*)emalloc(NAME_MAX);
    bzero(save_controller,NAME_MAX);
    save_method = (char*)emalloc(METHODNAMEMAX);
    bzero(save_method,METHODNAMEMAX);


    //对url地址进行切割，切割出模块 控制器 和路由
    request_uri = strtok(url_buff,"?");
    if(request_uri) {
        //获取模块
        module = strtok(request_uri, "/");
        if (!module) {
            php_error_docref(NULL, E_ERROR, "Module Error");
            return SFF_FALSE;
        }
        strcpy(module_buf,module);
//        handel->module = module_buf;
        memcpy(save_module,module,strlen(module));
        handel->module = save_module;

        //获取控制器
        controller = strtok(NULL, "/");
        if (!controller) {
            php_error_docref(NULL, E_ERROR, "Controller Error");
            return SFF_FALSE;
        }
        strcpy(controller_buf,controller);
//        handel->controller = controller_buf;
        strcpy(save_controller,controller);
        handel->controller = save_controller;

        //获取方法
        method = strtok(NULL, "/");
        if (!method) {
            php_error_docref(NULL, E_ERROR, "Method Error");
            return SFF_FALSE;
        }
        strcpy(method_buf,method);
//        handel->method = method_buf;
        strcpy(save_method,method);
        handel->method = save_method;

        //由于需要计算长度所以需要转化成数组，不能再用字符串常量区的地址
        //拆分出的模块 控制器 路由 放入全局变量
        SFF_G(current_module) = zend_string_init(module_buf,strlen(module_buf),0);
        SFF_G(current_controller) = zend_string_init(controller_buf,strlen(controller_buf),0);
        SFF_G(current_method) = zend_string_init(method_buf,strlen(method_buf),0);
    }
    return SFF_FALSE;
}

//SFF_BOOL sff_route_dispatch(struct sff_route_pack_ *handel)
//{
//
//    //在栈上申请一个新的内存,重新存储url
//    char url_buff[2048];
//    bzero(url_buff,sizeof(url_buff));
//
//
//    char *request_uri;
//
//    //模块
//    char *module;
//
//    //在栈上申请模块
//    char module_buf[MAXBUF];
//
//    //存储到结构体的模块地址
//    char *save_module;
//
//    //控制器
//    char *controller;
//
//    //在栈上申请控制器
//    char controller_buf[MAXBUF];
//
//    //存储到结构体的控制器地址
//    char *save_controller;
//
//    //方法
//    char *method;
//
//    //存储到结构体的方法地址
//    char *save_method;
//
//    //在栈上申请方法
//    char method_buf[MAXBUF];
//
//    zval *server,*uri = NULL;
//
//    char *url = NULL;
//
//    if (PG(auto_globals_jit)) {
//        zend_string *server_str = zend_string_init(ZEND_STRL("_SERVER"), 0);
//        zend_is_auto_global(server_str);
//        zend_string_release(server_str);
//    }
//
//    //获取$_SERVER变量
//    server = &PG(http_globals)[TRACK_VARS_SERVER];
//
//    //如果说对象是数组
//    if (Z_TYPE((*server)) == IS_ARRAY){
//        //发现$_SERVER中的REQUEST_URI
//        uri = zend_hash_str_find(Z_ARRVAL(PG(http_globals)[TRACK_VARS_SERVER]), "REQUEST_URI", strlen("REQUEST_URI"));
//        if(!uri)
//        {
//            php_error_docref(NULL, E_ERROR, "$_SERVER REQUEST_URI Error");
//        }
//        if (Z_TYPE(*(uri)) == IS_STRING){
//            url = ZSTR_VAL(Z_STR(*uri));
//            strcpy(url_buff,url);
//        } else{
//            php_error_docref(NULL, E_ERROR, "$_SERVER REQUEST_URI Is Not String");
//        }
//    }
//
//    if(!url)
//    {
//        php_error_docref(NULL, E_ERROR, "get url Error");
//    }
//
//
//    //格式化栈上的路由信息
//    bzero(module_buf,sizeof(module_buf));
//    bzero(controller_buf, sizeof(controller_buf));
//    bzero(method_buf, sizeof(method_buf));
//
//    //在堆上申请空间,并且将内容初始化
//    save_module = (char*)emalloc(NAME_MAX);
//    bzero(save_module,NAME_MAX);
//    save_controller = (char*)emalloc(NAME_MAX);
//    bzero(save_controller,NAME_MAX);
//    save_method = (char*)emalloc(METHODNAMEMAX);
//    bzero(save_method,METHODNAMEMAX);
//
//
//    //对url地址进行切割，切割出模块 控制器 和路由
//    request_uri = strtok(url_buff,"?");
//    if(request_uri) {
//        //获取模块
//        module = strtok(request_uri, "/");
//        if (!module) {
//            php_error_docref(NULL, E_ERROR, "Module Error");
//            return SFF_FALSE;
//        }
//        strcpy(module_buf,module);
////        handel->module = module_buf;
//        memcpy(save_module,module,strlen(module));
//        handel->module = save_module;
//
//        //获取控制器
//        controller = strtok(NULL, "/");
//        if (!controller) {
//            php_error_docref(NULL, E_ERROR, "Controller Error");
//            return SFF_FALSE;
//        }
//        strcpy(controller_buf,controller);
////        handel->controller = controller_buf;
//        strcpy(save_controller,controller);
//        handel->controller = save_controller;
//
//        //获取方法
//        method = strtok(NULL, "/");
//        if (!method) {
//            php_error_docref(NULL, E_ERROR, "Method Error");
//            return SFF_FALSE;
//        }
//        strcpy(method_buf,method);
////        handel->method = method_buf;
//        strcpy(save_method,method);
//        handel->method = save_method;
//
//        //由于需要计算长度所以需要转化成数组，不能再用字符串常量区的地址
//
//
//        //拆分出的模块 控制器 路由 放入全局变量
//        SFF_G(current_module) = zend_string_init(module_buf,strlen(module_buf),0);
//        SFF_G(current_controller) = zend_string_init(controller_buf,strlen(controller_buf),0);
//        SFF_G(current_method) = zend_string_init(method_buf,strlen(method_buf),0);
//    }
//    return SFF_FALSE;
//}

SFF_BOOL sff_route_run()
{

}

//设置sff 路由包的根目录
void sff_set_root_dir(sff_route_pack *handel,const char *root_dir)
{
    handel->root_dir = root_dir;
    SFF_G(root_dir) = zend_string_init(root_dir,strlen(root_dir),0);
}

SFF_BOOL destroy(sff_route_pack *handel)
{
    efree(handel->module);
    efree(handel->controller);
    efree(handel->method);
}

