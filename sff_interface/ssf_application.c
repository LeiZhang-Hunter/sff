//
// Created by zhanglei on 19-2-20.
//
#include "../sff_common.h"
//加载路由包 放在全局位置
sff_route_pack route_pack;
sff_route_pack *route_factory;

//工厂的构造函数

//构造函数
PHP_METHOD (SimSuperFactory, __construct) {

    zval *root_path = NULL;//this opetion begin single model
    zval server;//存储$_SERVER的结构体地址
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL_DEREF(root_path)
    ZEND_PARSE_PARAMETERS_END();

    //如果说没有传递参数那么默认为根路径，如果说传递了参数
    if(!root_path)
    {

        if (PG(auto_globals_jit)) {
            zend_string *server_str = zend_string_init(ZEND_STRL("_SERVER"), 0);
            zend_is_auto_global(server_str);
            zend_string_release(server_str);
        }

        server = PG(http_globals)[TRACK_VARS_SERVER];
        //检查是否是数组
        if (Z_TYPE(server) == IS_ARRAY)
        {
            //如果是数组检查是否存在DOCUMENT_ROOT这个选项
            root_path = zend_hash_str_find(Z_ARRVAL(PG(http_globals)[TRACK_VARS_SERVER]), "REQUEST_URI", strlen("REQUEST_URI"));
            if(root_path)
            {
                //将根目录更新到属性中
                zend_update_property(ssf_application_entry,getThis(),SFF_ROOT_DIR,strlen(SFF_ROOT_DIR),root_path);
            }else{
                php_error_docref(NULL, E_ERROR, "$_SERVER Of Document Root IS NULL");
            }
        }else{
            php_error_docref(NULL, E_ERROR, "$_SERVER IS NULL");
        }
    }else{
        if(Z_TYPE(*root_path) == IS_STRING)
        {
            zend_update_property(ssf_application_entry,getThis(),SFF_ROOT_DIR,strlen(SFF_ROOT_DIR),root_path);
        }else{
            php_error_docref(NULL, E_ERROR, "Param must be string");
        }
    }

    //查看主类是否被重复实例化，如果是的话，那么就不允许他重复运行
    if(SFF_G(run_status))
    {
        php_error_docref(NULL, E_ERROR, "the application has been run");
        RETURN_FALSE
    }

    SFF_G(run_status) = 1;



    //初始化路由包
    bzero(&route_pack,sizeof(route_pack));

    //初始化路由包地址
    route_factory = ssf_route_pack_init(&route_pack);


    //设置路由包的根目录
    route_factory->set_root_dir(route_factory,Z_STRVAL(*root_path));

    //派遣路由将路由放入全局变量
    route_factory->dispatch(route_factory);

    //将路由的默认模块默认方法以及默认控制器转化为zval然后更新到类的属性中
    zval default_module;
    bzero(&default_module,sizeof(default_module));
    ZVAL_NEW_STR(&default_module,SFF_G(current_module));

    zval default_controller;
    bzero(&default_controller,sizeof(default_controller));
    ZVAL_NEW_STR(&default_controller,SFF_G(current_controller));

    zval default_method;
    bzero(&default_method,sizeof(default_method));
    ZVAL_NEW_STR(&default_method,SFF_G(current_method));

    //将默认路由更新至类的属性中
    zend_update_property(ssf_application_entry,getThis(),SSF_DEFAULT_MODULE,strlen(SSF_DEFAULT_MODULE),&default_module);
    zend_update_property(ssf_application_entry,getThis(),SSF_DEFAULT_CONTROLLER,strlen(SSF_DEFAULT_CONTROLLER),&default_controller);
    zend_update_property(ssf_application_entry,getThis(),SSF_DEFAULT_METHOD,strlen(SSF_DEFAULT_METHOD),&default_method);
}

//获取request的模块
PHP_METHOD (SimSuperFactory, getModule)
{
    zval return_module;
    zval *read_method_result = zend_read_property(ssf_application_entry,getThis(),SSF_DEFAULT_MODULE,strlen(SSF_DEFAULT_MODULE),0,&return_module);
    if(!read_method_result)
    {
        php_error_docref(NULL, E_ERROR, "the module prototype error");
    }
    RETURN_ZVAL(read_method_result,1,0);
}

//获取request的控制器
PHP_METHOD (SimSuperFactory, getController)
{
    zval return_controller;
    zval *read_controller_result = zend_read_property(ssf_application_entry,getThis(),SSF_DEFAULT_CONTROLLER,strlen(SSF_DEFAULT_CONTROLLER),0,&return_controller);
    if(!read_controller_result)
    {
        php_error_docref(NULL, E_ERROR, "the controller prototype error");
    }
    RETURN_ZVAL(read_controller_result,1,0);
}

//获取request的方法
PHP_METHOD (SimSuperFactory, getMethod)
{
    zval return_method;
    zval *read_method_result = zend_read_property(ssf_application_entry,getThis(),SSF_DEFAULT_METHOD,strlen(SSF_DEFAULT_METHOD),0,&return_method);
    if(!read_method_result)
    {
        php_error_docref(NULL, E_ERROR, "the method prototype error");
    }
    RETURN_ZVAL(read_method_result,1,0);
}

//在请求到达前注入闭包函数
PHP_METHOD(SimSuperFactory,onRequestBefore)
{
    zval *before_hook = NULL;
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL_DEREF(before_hook)
    ZEND_PARSE_PARAMETERS_END();

    if (!ssf_check_zval_function(before_hook)) {
        php_error_docref(NULL, E_ERROR, "param must be closure function");
        RETURN_FALSE;
    }
    zend_update_property(ssf_application_entry, getThis(), SFF_REQUEST_BEFORE_HOOK, sizeof(SFF_REQUEST_BEFORE_HOOK) - 1, before_hook);

    RETURN_TRUE;
}

//在请求到达后注入闭包函数
PHP_METHOD(SimSuperFactory,onRequestEnd)
{
    zval *end_hook = NULL;
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL_DEREF(end_hook)
    ZEND_PARSE_PARAMETERS_END();

    if (!ssf_check_zval_function(end_hook)) {
        php_error_docref(NULL, E_ERROR, "param must be closure function");
        RETURN_FALSE;
    }
    zend_update_property(ssf_application_entry, getThis(), SFF_REQUEST_AFTER_HOOK, sizeof(SFF_REQUEST_AFTER_HOOK) - 1, end_hook);

    RETURN_TRUE;
}

//获取根路径
PHP_METHOD(SimSuperFactory,getRootDir)
{
    zval root_dir;
    zval *result_root_dir = zend_read_property(ssf_application_entry,getThis(),SFF_ROOT_DIR,strlen(SFF_ROOT_DIR),0,&root_dir);
    if(!result_root_dir)
    {
        php_error_docref(NULL, E_ERROR, "the root dir error");
    }
    RETURN_ZVAL(result_root_dir,1,0);
}

//注入工厂配置
PHP_METHOD(SimSuperFactory,setConfig)
{
    zval *sff_config;
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL_DEREF(sff_config)
    ZEND_PARSE_PARAMETERS_END();
    if(!sff_config)
    {
        php_error_docref(NULL, E_ERROR, "the config is not null");
    }

    if(Z_TYPE(*sff_config) != IS_ARRAY)
    {
        php_error_docref(NULL, E_ERROR, "the param must be array");
    }

    zend_update_static_property(ssf_application_entry,SFF_REQUEST_CONFIG,strlen(SFF_REQUEST_CONFIG),sff_config);

    zval* heap_store = emalloc(sizeof(zval));
    memcpy(heap_store,sff_config,sizeof(zval));
    //同步将一份数据复制到全局变量
    SFF_G(config) = heap_store;
}

//设置项目主要文件夹
PHP_METHOD(SimSuperFactory,setMain)
{
    zval *sff_main_folder;
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL_DEREF(sff_main_folder)
    ZEND_PARSE_PARAMETERS_END();
    if(!sff_main_folder)
    {
        php_error_docref(NULL, E_ERROR, "the config is not null");
    }

    if(Z_TYPE(*sff_main_folder) != IS_STRING)
    {
        php_error_docref(NULL, E_ERROR, "the param must be string");
    }


    //如果说没有设置需要重置内存地址
    if(!SFF_G(main_folder)) {
        //申请一块内存地址
        char *main_dir = emalloc(NAME_MAX);

        //写入全局变量
        bzero(main_dir, 1024);
        strcpy(main_dir,Z_STRVAL(*sff_main_folder));
        SFF_G(main_folder) = Z_STRVAL(*sff_main_folder);
    }else{
        //这个项目主目录已经被设置了
        php_error_docref(NULL, E_ERROR, "the main folder has been set");
    }
}

//获取注入的工厂配置
PHP_METHOD(SimSuperFactory,getConfig)
{
    zval sff_config;
//    zval *read_config = zend_read_property(ssf_application_entry,getThis(),SFF_REQUEST_CONFIG,strlen(SFF_REQUEST_CONFIG),0,&sff_config);
    if(!SFF_G(config))
    {
        php_error_docref(NULL, E_ERROR, "the config is null");
    }
    RETURN_ZVAL(SFF_G(config),1,0);
}



//运行超级工厂
PHP_METHOD (SimSuperFactory, run)
{

    /*参数初始化*/
    //截取路由包地址
    //在栈上申请一块内存用来存储控制器路径
    char controller_file_dir[MAXPATHLEN];
    zval            args[1];//自定义参数
    zval null_result_before_hook;
    zval *before_hook;//后置钩子函数变量
    zval *end_hook;//前置钩子函数变量
    zval server_fun_result;//路由函数运行返回值
    zval method_name;//路由方法名
    zval server_zval;//路由服务的对象变量
    zval null_result_finish_hook;//后置闭包函数的返回变量
    HashTable * internal_class_table;//内核的class_table
    zval *service_fun;//路由函数
    char class_name[MAXPATHLEN];
    bzero(class_name,sizeof(class_name));

    bzero(&controller_file_dir,sizeof(controller_file_dir));

//    //如果说没有设置主目录
    if(!SFF_G(main_folder)) {
        sprintf(controller_file_dir, "%s/controllers/%s/%s.php", route_factory->root_dir, route_factory->module,
                route_factory->controller);
    }else{
        //设置了主目录的情况
        sprintf(controller_file_dir, "%s/%s/controllers/%s/%s.php", route_factory->root_dir,SFF_G(main_folder),route_factory->module,
                route_factory->controller);
    }

    //检查是否是文件
    if(access(controller_file_dir,F_OK) == -1)
    {
        php_error_docref(NULL, E_ERROR, "route file is not exist");
    }
//
//
//
//    //执行前闭包函数运行
    before_hook = ssf_ce_read_prototype(ssf_application_entry,getThis(),SFF_REQUEST_BEFORE_HOOK,(int)strlen(SFF_REQUEST_BEFORE_HOOK));
//
    if(!before_hook)
    {
        php_error_docref(NULL, E_ERROR, "before hook is error");
    }
//
    if(ssf_check_zval_function(before_hook))
    {
        args[0] = *getThis();
        call_user_function_ex(EG(function_table), getThis(), before_hook, &null_result_before_hook, 1, args, 0, NULL);
    }

    //引入php文件
    zval include_result;
    include_file(controller_file_dir,&include_result);

    //执行路由
    //获取内核中的class_table
    internal_class_table = EG(class_table);

    //拼接类名
    sprintf(class_name,"%s\\%s",route_factory->module,route_factory->controller);
    //检查类是否存在
    zend_class_entry *service_entry = zend_fetch_class(zend_string_tolower(zend_string_init(class_name,strlen(class_name),0)),ZEND_FETCH_CLASS_AUTO);
    if(service_entry == NULL)
    {
        php_error_docref(NULL, E_ERROR, "controller class error");
    }

    //检查类中是否有这个方法
    service_fun = zend_hash_find(&service_entry->function_table,zend_string_tolower(SFF_G(current_method)));
    if(!service_fun)
    {
        php_error_docref(NULL, E_ERROR, "method function error");
    }





    args[0] = *getThis();
    //执行路由
    object_init_ex(&server_zval,service_entry);
    //执行构造函数
    zval construct;
    zend_string* zend_construct = zend_string_init("__construct",strlen("__construct"),0);
    ZVAL_NEW_STR(&construct,zend_string_tolower(zend_construct));
    call_user_function_ex(EG(function_table),&server_zval, &construct, &server_fun_result, 0, NULL, 0,NULL);
    zend_string_release(zend_construct);

    //注入框架主类实例
    zend_update_property(factory_controller_entry,&server_zval,SFF_APPLICATION_INSTANCE,strlen(SFF_APPLICATION_INSTANCE),getThis());


    //注入加载类
    //执行路由函数
    ZVAL_NEW_STR(&method_name,zend_string_tolower(SFF_G(current_method)));
    call_user_function_ex(EG(function_table),&server_zval, &method_name, &server_fun_result, 1, args, 0,NULL);

    //执行后闭包函数运行
    end_hook = ssf_ce_read_prototype(ssf_application_entry,getThis(),SFF_REQUEST_AFTER_HOOK,(int)strlen(SFF_REQUEST_AFTER_HOOK));

    if(ssf_check_zval_function(end_hook))
    {
        args[0] = *getThis();
        call_user_function_ex(EG(function_table), getThis(), end_hook, &null_result_finish_hook, 1, args, 1, NULL);
    }

    //释放掉在堆中的内存空间
    route_factory->destroy(route_factory);
}

//析构函数
PHP_METHOD (SimSuperFactory, __destruct) {
}

//定义SimSuperFactory类的内容
const zend_function_entry sff_application_struct[] = {
        PHP_ME(SimSuperFactory, __construct, config_param_struct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(SimSuperFactory, getModule, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, getController, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, getMethod, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, onRequestBefore, request_before_hook, ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, onRequestEnd, request_after_hook, ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, getRootDir, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, setConfig, request_after_hook, ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, setMain, sff_main_folder_struct, ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, getConfig, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, run, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(SimSuperFactory, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
        PHP_FE_END
};
//初始化ssf_application函数
void ssf_application_init()
{
    //加载所需要的配置类
    INIT_CLASS_ENTRY(ssf_application, "SimSuperFactory", sff_application_struct);
    ssf_application_entry = zend_register_internal_class_ex(&ssf_application, NULL);
    //声明默认模块属性
    zend_declare_property_null(ssf_application_entry,SSF_DEFAULT_MODULE,strlen(SSF_DEFAULT_MODULE),ZEND_ACC_PRIVATE TSRMLS_CC);
    //声明默认控制器属性
    zend_declare_property_null(ssf_application_entry,SSF_DEFAULT_CONTROLLER,strlen(SSF_DEFAULT_CONTROLLER),ZEND_ACC_PRIVATE TSRMLS_CC);
    //声明默认方法属性
    zend_declare_property_null(ssf_application_entry,SSF_DEFAULT_METHOD, strlen(SSF_DEFAULT_METHOD),ZEND_ACC_PRIVATE TSRMLS_CC);
    //声明根目录属性
    zend_declare_property_null(ssf_application_entry,SFF_ROOT_DIR, strlen(SFF_ROOT_DIR),ZEND_ACC_PRIVATE TSRMLS_CC);
    //声明前置钩子
    zend_declare_property_null(ssf_application_entry,SFF_REQUEST_BEFORE_HOOK, strlen(SFF_REQUEST_BEFORE_HOOK),ZEND_ACC_PRIVATE TSRMLS_CC);
    //声明后置钩子
    zend_declare_property_null(ssf_application_entry,SFF_REQUEST_AFTER_HOOK, strlen(SFF_REQUEST_AFTER_HOOK),ZEND_ACC_PRIVATE TSRMLS_CC);
    //配置属性
//    zend_declare_property_null(ssf_application_entry,SFF_REQUEST_CONFIG, strlen(SFF_REQUEST_CONFIG),ZEND_ACC_PRIVATE TSRMLS_CC);

    zend_declare_property_null(ssf_application_entry,SFF_REQUEST_CONFIG, strlen(SFF_REQUEST_CONFIG),ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);
}
