//
// Created by zhanglei on 19-5-27.
//

#ifndef SFF_SFF_PDO_POOL_H
#define SFF_SFF_PDO_POOL_H

#endif //SFF_SFF_PDO_POOL_H

//在栈上申请内存
zend_class_entry pdo_pool_entry;

//栈上申请内存的实例 存储为一个全局变量
zend_class_entry *pdo_pool_entry_address;

//定义接口
PHP_METHOD (SffPdoPool, __construct);

void sff_pdo_pool_init();