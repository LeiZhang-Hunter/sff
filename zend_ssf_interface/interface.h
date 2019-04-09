//
// Created by root on 18-12-1.
//

#ifndef SFF_INTERFACE_H
#define SFF_INTERFACE_H

#endif //SFF_INTERFACE_H

#ifndef SFF_SFF_PROCESS_H

#include "../tool/sff_process.h"

#endif //SFF_SFF_PROCESS_H


zval* sff_ce_read_prototype(zend_class_entry *scope, zval *object,const char* pro_name,int strlen);

int sff_check_zval_function(zval* check_param);

//将进程块转化为数组
SFF_BOOL convert_process_block_zval_array(process_block *block,zval *process_array);
