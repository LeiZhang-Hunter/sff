/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_sff.h"

#include "sff_common.h"

extern const zend_function_entry config_class_struct[];

/* If you declare any globals in php_sff.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(sff)
*/

/* True global resources - no need for thread safety here */
static int le_sff;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("sff.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_sff_globals, sff_globals)
    STD_PHP_INI_ENTRY("sff.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_sff_globals, sff_globals)
PHP_INI_END()
*/


/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_sff_compiled(string arg)
   Return a string to confirm that the module is compiled in */

PHP_FUNCTION (confirm_sff_compiled) {

}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_sff_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_sff_init_globals(zend_sff_globals *sff_globals)
{
	sff_globals->global_value = 0;
	sff_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */

static void sff_globals_init(zend_sff_globals *sff_globals)
{
    bzero(sff_globals,sizeof(zend_sff_globals));
}

static void sff_globals_destroy(zend_sff_globals *sff_globals)
{
    //依次释放掉没用的内容
//    if(sff_globals->current_module) {
//        zend_string_release(sff_globals->current_module);
//    }
//
//    if(sff_globals->current_controller) {
//        zend_string_release(sff_globals->current_controller);
//    }
//
//    if(sff_globals->current_method) {
//        zend_string_release(sff_globals->current_method);
//    }

    //清理结构体
    bzero(sff_globals,sizeof(zend_sff_globals));
}

PHP_MINIT_FUNCTION (sff) {
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */

    //加载配置
    REGISTER_INI_ENTRIES();

    //初始化我们定义的全局变量
    ZEND_INIT_MODULE_GLOBALS(sff,sff_globals_init,sff_globals_destroy);

    //初始化配置类
    ssf_config_init();

    //初始化工厂控制器
    factory_application_init();

    //初始化主类
    ssf_application_init();

    //加载自动加载类
    sff_load_init();

    factory_container_init();
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION (sff) {
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION (sff) {
#if defined(COMPILE_DL_SFF) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    ZEND_INIT_MODULE_GLOBALS(sff,sff_globals_init,sff_globals_destroy);
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION (sff) {
    return SUCCESS;
}
/* }}} */



/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION (sff) {
    php_info_print_table_start();
    php_info_print_table_header(2, "sff support", "enabled");
    php_info_print_table_end();

    //init Config entry

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}

//const zend_function_entry config_class_struct[] = {
//		PHP_ME(Config,getInstance,NULL,ZEND_ACC_PUBLIC)
//};


/* }}} */

/* {{{ sff_functions[]
 *
 * Every user visible function must have an entry in sff_functions[].
 */
const zend_function_entry sff_functions[] = {
        PHP_FE(confirm_sff_compiled, NULL)        /* For testing, remove later. */
        PHP_FE_END    /* Must be the last line in sff_functions[] */
};
/* }}} */

/* {{{ sff_module_entry
 */
zend_module_entry sff_module_entry = {
        STANDARD_MODULE_HEADER,
        "sff",
        sff_functions,
        PHP_MINIT(sff),
        PHP_MSHUTDOWN(sff),
        PHP_RINIT(sff),        /* Replace with NULL if there's nothing to do at request start */
        PHP_RSHUTDOWN(sff),    /* Replace with NULL if there's nothing to do at request end */
        PHP_MINFO(sff),
        PHP_SFF_VERSION,
        STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SFF
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(sff)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
