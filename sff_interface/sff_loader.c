//
// Created by zhanglei on 19-2-23.
//

#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"

#endif


const zend_function_entry sff_loader_struct[] = {
        PHP_ME(SuperAutoLoader, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(SuperAutoLoader, addNameSpace, namespace_struct, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(SuperAutoLoader, start_monitoring, NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(SuperAutoLoader, auto_monitoring, namespace_store_struct, ZEND_ACC_STATIC | ZEND_ACC_PUBLIC)
        PHP_ME(SuperAutoLoader, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
        PHP_FE_END
};

PHP_METHOD(SuperAutoLoader,__construct)
{

}

//添加命名空间
PHP_METHOD(SuperAutoLoader,addNameSpace)
{
    zval *namespace_key;//键
    zval *namespace_value;//值
    int namespace_type;//命名空间属性的类型
    zval namespace;
    zval *namespace_store;//namespace存储结果
    zval namespace_array;//初始化的hashtable
    zval* factory_instance_store;
    zval *tmp;
    zval value;
    //参数声明
    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_ZVAL(namespace_key)
            Z_PARAM_ZVAL(namespace_value)
    ZEND_PARSE_PARAMETERS_END();



    if(!namespace_key)
    {
        RETURN_FALSE
    }

    if(!namespace_value)
    {
        RETURN_FALSE
    }

    //检查namespace_key必须是字符串
    if(Z_TYPE(*namespace_key) != IS_STRING)
    {
        php_error_docref(NULL, E_ERROR, "namespace_key must be string");
    }

    //检查namespace_value必须是字符串
    if(Z_TYPE(*namespace_value) != IS_STRING)
    {
        php_error_docref(NULL, E_ERROR, "namespace_value must be string");
    }

    //namespace_value必须是一个文件夹否则抛出异常
    if(is_dir(ZSTR_VAL(Z_STR(*namespace_value))) == SFF_FALSE)
    {
        php_error_docref(NULL, E_ERROR, "namespace_value must be dir");
    }

    //复制变量防止地址安全问题
    ZVAL_COPY(&value,namespace_value);


//    factory_instance_store = zend_read_property(Z_OBJCE_P(getThis()),getThis(),SFF_NAMESPANCE,sizeof(SFF_NAMESPANCE)-1,0,&namespace);
    factory_instance_store = zend_read_static_property(sff_loader_entry,SFF_NAMESPANCE,sizeof(SFF_NAMESPANCE)-1,0);
//
    if(factory_instance_store) {
        namespace_type = Z_TYPE(*factory_instance_store);
        if (namespace_type == IS_NULL) {
            //第一次加载必须要初始化namespace为一个数组
            //初始化hashtable
            bzero(&namespace_array,sizeof(namespace_array));

            array_init(&namespace_array);
            if(Z_TYPE(namespace_array) == IS_ARRAY)
            {

                zend_hash_update(Z_ARRVAL_P(&namespace_array),(Z_STR(*namespace_key)),&value);

                //如果初始化的是一个数组
                zend_update_static_property(sff_loader_entry,SFF_NAMESPANCE,strlen(SFF_NAMESPANCE),&namespace_array);

            }else{
                //提示错误信息
                php_error_docref(NULL, E_ERROR, "namespace array init data failed");
            }

        } else if (namespace_type == IS_ARRAY){
            //更新这个静态属性
            zend_hash_update(Z_ARRVAL_P(factory_instance_store),(Z_STR(*namespace_key)),&value);
            zend_update_static_property(sff_loader_entry,SFF_NAMESPANCE,strlen(SFF_NAMESPANCE),factory_instance_store);
        }else{
            //提示错误信息
            php_error_docref(NULL, E_ERROR, "namespace save data must be array");
        }
    }else{
        //提示错误信息
        php_error_docref(NULL, E_ERROR, "factory instance store must be array");
    }
}

//开启异步监控过程无感知
PHP_METHOD(SuperAutoLoader,start_monitoring)
{
//    zval *monitor_function;

    //参数声明
//    ZEND_PARSE_PARAMETERS_START(1, 1)
//            Z_PARAM_OPTIONAL
//            Z_PARAM_ZVAL_DEREF(monitor_function)
//    ZEND_PARSE_PARAMETERS_END();


    //检测是否是闭包函数
//    if(Z_TYPE(*monitor_function) != IS_OBJECT)
//    {
//        php_error_docref(NULL, E_ERROR, "monitoring param must be function call");
//    }

    zend_string* autoload;
    zval *auto_handle;
    zval function_name;//函数的名字
    zval return_result;
    zval            args[1];//自定义参数
    zval param_array;
    //从函数列表里找到spl_autoload_register然后运行他
    autoload = zend_string_init(SFF_SPL_AUTO_LOAD_FUNCTION,strlen(SFF_SPL_AUTO_LOAD_FUNCTION),0);
    auto_handle = zend_hash_find(EG(function_table),zend_string_tolower(autoload));
    ZVAL_NEW_STR(&function_name,autoload);
    zend_string_release(autoload);


    //如果说可以找到spl_autoload
    if(auto_handle)
    {
        //初始化一个数组
        array_init(&param_array);

        add_index_string((&param_array),0,SFF_LOADER_CLASS);
        add_index_string((&param_array),1,SFF_AUTO_LOAD_FUNCTION);
        args[0] = param_array;
        //执行自动加载函数 spl_autoload_register
        call_user_function_ex(EG(function_table),NULL, &function_name, &return_result, 1, args, 0,NULL);
        RETURN_ZVAL(&return_result,1,0);
    }else{
        php_error_docref(NULL, E_ERROR, "find spl_autoload_register error");
    }
}

//自动加载函数
PHP_METHOD(SuperAutoLoader,auto_monitoring)
{
    zval *namespace_key;//穿过来的参数
    char namespace[MAXPATHLEN];//定义命名空间大小
    char *zval_namespace;//参数的值
    char *find_namespace;//要寻找的namespace
    char *file_name;//要寻找的namespace
    char save_dir[MAXPATHLEN];
    char include_file_dir[MAXPATHLEN];//文件路径
    zval* namespace_value;//存储的值
    zval* factory_instance_store;
    zval tmp_namespace;
    //参数声明
    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ZVAL_DEREF(namespace_key)
    ZEND_PARSE_PARAMETERS_END();

    //如果参数是字符串
    if(Z_TYPE(*namespace_key) == IS_STRING)
    {
        //初始化这块地址
        bzero(namespace,sizeof(namespace));

        //获取命名空间
        zval_namespace = Z_STRVAL(*namespace_key);
        strcpy(namespace,zval_namespace);

        //分割出命名空间
        find_namespace = strtok(namespace,"\\");

        if(!find_namespace)
        {
            RETURN_FALSE;
        }

        //分割出要寻找的文件名子
        file_name = strtok(NULL,"\\");
        if(!file_name)
        {
            RETURN_FALSE;
        }

        //读取属性的值
        //factory_instance_store = zend_read_property(Z_OBJCE_P(getThis()),getThis(),SFF_NAMESPANCE,sizeof(SFF_NAMESPANCE)-1,0,&tmp_namespace);
        factory_instance_store = zend_read_static_property(sff_loader_entry,SFF_NAMESPANCE,sizeof(SFF_NAMESPANCE)-1,0);


        if(factory_instance_store)
        {
            //如果属性被初始化过并且是数组
            if(Z_TYPE(*factory_instance_store) == IS_ARRAY)
            {
                //从命名空间中找到要找的命名空间路径
                zend_string *find_str = zend_string_init(find_namespace,strlen(find_namespace),0);
                if((namespace_value = zend_hash_find(Z_ARRVAL_P(factory_instance_store),find_str)))
                {
                    bzero(include_file_dir,sizeof(include_file_dir));
//                    zend_string_release(find_str);//释放掉内存

                    strcpy(save_dir,Z_STRVAL(*namespace_value));

                    //如果说找到了这个值,那么拼接组装成路径进行引入
                    sprintf(include_file_dir,"%s/%s.php",save_dir,file_name);

                    //如果说存在文件开始执行加载
                    //输出打印
//                    php_printf("load_file:%s......\n",include_file_dir);
                    if(access(include_file_dir,F_OK) == 0)
                    {
                        //加载路径
                        include_file(include_file_dir,&tmp_namespace);
//                        RETURN_ZVAL(factory_instance_store,1,0);
                    }
                }
                zend_string_release(find_str);//释放掉内存

            }
        }

    }
    RETURN_FALSE
}

PHP_METHOD(SuperAutoLoader,__destruct)
{

}

//初始化ssf_application函数
void sff_load_init()
{
    //加载所需要的配置类
    INIT_CLASS_ENTRY(sff_loader, "SuperAutoLoader", sff_loader_struct);
    sff_loader_entry = zend_register_internal_class_ex(&sff_loader, NULL);
    //初始化一个空的数组把这个属性定义为一个空数组

    zval namespace_array;
    ZVAL_NULL(&namespace_array);
    //声明默认模块属性
    zend_declare_property_null(sff_loader_entry,SFF_NAMESPANCE,strlen(SFF_NAMESPANCE),ZEND_ACC_STATIC TSRMLS_CC);
}