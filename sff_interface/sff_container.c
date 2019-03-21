//
// Created by zhanglei on 19-3-15.
//
#include "../sff_common.h"

const zend_function_entry factory_container_struct[] = {
        PHP_ME(SffContainer, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(SffContainer, setConfig, NULL,  ZEND_ACC_PUBLIC)
        PHP_ME(SffContainer, run, NULL,  ZEND_ACC_PUBLIC)
        PHP_ME(SffContainer, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
        PHP_FE_END
};

super_container container_instance;


//初始化容器配置
PHP_METHOD (SffContainer, __construct)
{
    //初始化这一个结构体
    super_container_init(&container_instance);
}

//设置容器的配置选项
PHP_METHOD (SffContainer, setConfig)
{
    zval *config = NULL;//this opetion begin single model
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_ARRAY(config)
    ZEND_PARSE_PARAMETERS_END();

    //循环传进来的数组,将数组的值放入结构体之中
    zval *config_item;
    zend_string *config_key;
    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(config), config_key, config_item) {
                if (config_key == NULL) {
                    continue;
                }
                container_instance.set_container_config(&container_instance,config_key,config_item);
            } ZEND_HASH_FOREACH_END();

//    php_printf("%s\n",container_instance.user);
//    php_printf("%s\n","end");
}

//运行容器
PHP_METHOD (SffContainer, run)
{

}


PHP_METHOD (SffContainer, __destruct)
{

}

void factory_container_init()
{
    //加载所需要的配置类
    INIT_CLASS_ENTRY(factory_container, "SffContainer", factory_container_struct);
    factory_container_entry = zend_register_internal_class_ex(&factory_container, NULL);

    //声明默认模块属性
}