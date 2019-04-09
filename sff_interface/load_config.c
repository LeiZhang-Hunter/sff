//
// Created by root on 18-11-18.
//
#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"

#endif


const zend_function_entry config_class_struct[] = {
        PHP_ME(Config, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(Config, getInstance, config_option_struct, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(Config, initConfig, config_path_struct, ZEND_ACC_PUBLIC)
        PHP_ME(Config, regInitBeforeStep, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(Config, regInitAfterStep, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(Config, getConfig, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(Config, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
        PHP_FE_END
};

extern zend_class_entry *config_ce;
zval config_instance;


PHP_METHOD (Config, __construct) {

}

//get Instance
PHP_METHOD (Config, getInstance) {
    zval * single_option = NULL;//this opetion begin single model
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL_DEREF(single_option)
    ZEND_PARSE_PARAMETERS_END();


    if (!single_option) {
        zval
        false_flag;
        ZVAL_FALSE(&false_flag);
        single_option = &false_flag;
    }

    if (Z_TYPE(*single_option) != IS_FALSE && Z_TYPE(*single_option) != IS_TRUE) {
        php_error_docref(NULL, E_WARNING, "param must be bool");
        RETURN_FALSE;
    }

    if (Z_TYPE(config_instance) == IS_OBJECT && Z_TYPE(*single_option) == IS_TRUE) {
        RETURN_ZVAL(&config_instance, 1, 0);
    } else {
        object_init_ex(&config_instance, config_ce);
        RETURN_ZVAL(&config_instance, 1, 0);
    }
}

//inject before init function
PHP_METHOD (Config, regInitBeforeStep) {
    zval * beforeFunction = NULL;
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL_DEREF(beforeFunction)
    ZEND_PARSE_PARAMETERS_END();

    if (!sff_check_zval_function(beforeFunction)) {
        php_error_docref(NULL, E_ERROR, "param must be closure function");
        RETURN_FALSE;
    }
    zend_update_property(config_ce, getThis(), SFF_BEFORE_HOOK, sizeof(SFF_BEFORE_HOOK) - 1, beforeFunction);

    RETURN_TRUE;
}

//inject before finish function
PHP_METHOD (Config, regInitAfterStep) {
    zval * beforeFunction = NULL;
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL_DEREF(beforeFunction)
    ZEND_PARSE_PARAMETERS_END();

    if (!sff_check_zval_function(beforeFunction)) {
        php_error_docref(NULL, E_ERROR, "param must be closure function");
        RETURN_FALSE;
    }
    zend_update_property(config_ce, getThis(), SFF_FINISH_HOOK, sizeof(SFF_FINISH_HOOK) - 1, beforeFunction);
    RETURN_TRUE;
}


//init Config
PHP_METHOD (Config, initConfig) {
    zval * config_path = NULL;//folder dir
    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(config_path)
    ZEND_PARSE_PARAMETERS_END();


    char config_dir[MAXPATHLEN];
    zval config_collect;
    char *folder_level[100] = {};
    zval * before_hook;//前置钩子
    zval * end_hook;//后置钩子

    //初始化字符串buffer
    bzero(config_dir,sizeof(config_dir));

    if (!config_path) {
        //get file folder
        char path[MAXPATHLEN];
        get_file_execute_dir(path);
        if(SFF_G(main_folder)) {
            php_sprintf(config_dir, "%s/%s/", path,SFF_G(main_folder));
        }else{
            php_sprintf(config_dir, "%s/",  path);
        }
        if (!opendir(config_dir)) {
            php_error_docref(NULL, E_ERROR, "Config Dir Is Not Exist");
            RETURN_FALSE;
        }
    } else {
        if (Z_TYPE(*config_path) != IS_STRING) {
            php_error_docref(NULL, E_ERROR, "param must be string");
            RETURN_FALSE;
        }

        //写入
        strcpy(config_dir,Z_STRVAL(*config_path));
    }

    array_init(&config_collect);
    HashTable * config_collect_table = Z_ARRVAL_P(&config_collect);

    before_hook = sff_ce_read_prototype(config_ce, getThis(), SFF_BEFORE_HOOK, sizeof(SFF_BEFORE_HOOK) - 1);

    if(Z_TYPE(*before_hook) != IS_NULL) {
        if (sff_check_zval_function(before_hook)) {
            zval
            args[1];
            zval
            null_result_before_hook;
            args[0] = *getThis();
            call_user_function_ex(EG(function_table), getThis(), before_hook, &null_result_before_hook, 1, args, 0,
                                  NULL);
        }
    }


    get_file_data_to_hash_table(config_collect_table, config_dir);


    zend_update_property(config_ce, getThis(), SFF_CONFIG_DATA, sizeof(SFF_CONFIG_DATA) - 1, &config_collect);

    end_hook = sff_ce_read_prototype(config_ce, getThis(), SFF_FINISH_HOOK, sizeof(SFF_FINISH_HOOK) - 1);

    if(Z_TYPE(*end_hook) != IS_NULL) {
        if (sff_check_zval_function(end_hook)) {
            zval
            args[1];
            zval
            null_result_finish_hook;
            args[0] = *getThis();
            call_user_function_ex(EG(function_table), getThis(), end_hook, &null_result_finish_hook, 1, args, 0, NULL);
        }
    }

    RETURN_TRUE
}

PHP_METHOD (Config, getConfig) {
    zval
    data;
    zval * ret;
    ret = zend_read_property(config_ce,
                             getThis(),
                             SFF_CONFIG_DATA,
                             sizeof(SFF_CONFIG_DATA) - 1,
                             0,
                             &data
    );

    RETURN_ZVAL(ret, 1, 0)
}

PHP_METHOD (Config, __destruct) {

}

void ssf_config_init() {
    //加载所需要的配置类
    INIT_CLASS_ENTRY(config, "Config", config_class_struct);
    config_ce = zend_register_internal_class_ex(&config, NULL);
    zend_declare_property_null(config_ce, SFF_BEFORE_HOOK, sizeof(SFF_BEFORE_HOOK) - 1, ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_null(config_ce, SFF_FINISH_HOOK, sizeof(SFF_FINISH_HOOK) - 1, ZEND_ACC_PRIVATE TSRMLS_CC);
    zend_declare_property_null(config_ce, SFF_CONFIG_DATA, sizeof(SFF_CONFIG_DATA) - 1, ZEND_ACC_PUBLIC TSRMLS_CC);
}