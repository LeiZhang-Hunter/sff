//
// Created by root on 18-12-1.
//

//check zval is function
#include "../sff_common.h"
int ssf_check_zval_function(zval* check_param)
{
    if(!check_param)
    {
        return 0;
    }
    char *func_name = NULL;
    zend_string *key = NULL;
    zend_fcall_info_cache *func_cache = (zend_fcall_info_cache *) emalloc(sizeof(zend_fcall_info_cache));
    if (!zend_is_callable_ex(check_param, NULL, 0, &key, func_cache, NULL)){
        efree(func_name);
        zend_string_release(key);
        return 0;
    }
    zend_string_release(key);
    efree(func_name);
    return 1;
}

zval* ssf_ce_read_prototype(zend_class_entry *scope, zval *object,const char* pro_name,int strlen)
{
    zval val;
    return zend_read_property(scope,object,pro_name,strlen,0,&val);
}