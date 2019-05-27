//
// Created by zhanglei on 19-2-23.
//
//定义SimSuperFactory类的内容
#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"

#endif

#include <sys/mman.h>


const zend_function_entry factory_application_struct[] = {
        PHP_ME(SffFactoryController, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(SffFactoryController, getFactoryInstance, NULL,  ZEND_ACC_PUBLIC)
        PHP_ME(SffFactoryController, createMem, NULL,  ZEND_ACC_PUBLIC)
        PHP_ME(SffFactoryController, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
        PHP_FE_END
};

PHP_METHOD(SffFactoryController,__construct)
{

}
zval *ptr;

//
PHP_METHOD(SffFactoryController,getFactoryInstance)
{
    RETURN_ZVAL(ptr,1,0);
}

PHP_METHOD(SffFactoryController,createMem)
{
    int fd = open("/home/zhanglei/test.mem",O_RDWR);
    if(fd <= 0)
    {
        php_error_docref(NULL, E_WARNING, "open file error");
        RETURN_FALSE
    }
    php_printf("open fd:%d\n",fd);
    zval arr;
    array_init(&arr);
    int zero = 0;
    size_t res = write(fd,&arr,sizeof(zval));
    php_printf("write bytes:%ld,errno:%d\n",res,errno);
    ptr = mmap(NULL,sizeof(zval),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);

    RETURN_ZVAL(ptr,1,0);
}


PHP_METHOD(SffFactoryController,__destruct)
{

}

//初始化ssf_application函数
void factory_application_init()
{
    //加载所需要的配置类
    INIT_CLASS_ENTRY(factory_controller, "SffFactoryController", factory_application_struct);
    factory_controller_entry = zend_register_internal_class_ex(&factory_controller, NULL);
    //声明默认模块属性
    zend_declare_property_null(factory_controller_entry,SFF_APPLICATION_INSTANCE,strlen(SFF_APPLICATION_INSTANCE),ZEND_ACC_PUBLIC TSRMLS_CC);
}