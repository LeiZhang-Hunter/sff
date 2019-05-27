//
// Created by zhanglei on 19-5-27.
//
#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"
#endif

const zend_function_entry pdo_pool_struct[] = {
        PHP_ME(SffPdoPool, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_FE_END
};


PHP_METHOD (SffPdoPool, __construct)
{

}

void sff_pdo_pool_init()
{
    INIT_CLASS_ENTRY(pdo_pool_entry, "SffPdoPool", pdo_pool_struct);
    pdo_pool_entry_address = zend_register_internal_class_ex(&pdo_pool_entry, NULL);
}