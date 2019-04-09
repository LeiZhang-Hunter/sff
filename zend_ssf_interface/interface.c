//
// Created by root on 18-12-1.
//

//check zval is function
#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"

#endif
int sff_check_zval_function(zval* check_param)
{
    if(!check_param)
    {
        return SFF_FALSE;
    }
    char *func_name = NULL;
    zend_string *key = NULL;
    zend_fcall_info_cache *func_cache = (zend_fcall_info_cache *) emalloc(sizeof(zend_fcall_info_cache));
    bzero(func_cache, sizeof(zend_fcall_info_cache));
    if (!zend_is_callable_ex(check_param, NULL, 0, &key, func_cache, NULL)){
        efree(func_name);
        zend_string_release(key);
        return SFF_FALSE;
    }
    zend_string_release(key);
    efree(func_name);
    return SFF_TRUE;
}

zval* sff_ce_read_prototype(zend_class_entry *scope, zval *object,const char* pro_name,int strlen)
{
    zval val;
    return zend_read_property(scope,object,pro_name,strlen,0,&val);
}

//把进程块结构体转化成数组传入回调函数
SFF_BOOL convert_process_block_zval_array(process_block *block,zval *process_array)
{
    //重组参数将他放入到zval的结构体中
    zval process_name;//名字
    zval process_pid;//pid
    zval process_exit_code;//退出码
    zval process_sig_no;//中断信号
    zval process_run_state;//运行状态
    zval process_index;//进程池索引
    char process_name_back[MAXPATHLEN];
    char process_start_cmd[MAXPATHLEN];
    char process_stop_cmd[MAXPATHLEN];
    //初始化变量为一个空的数组
    array_init(process_array);
    //清理字符串缓冲的buffer防止出现安全问题
    bzero(process_name_back, sizeof(process_name_back));
    bzero(process_start_cmd, sizeof(process_start_cmd));
    bzero(process_stop_cmd, sizeof(process_stop_cmd));
    //复制字符串到缓冲区
    strcpy(process_name_back,block->process_name);
    strcpy(process_start_cmd,block->start_cmd);
    strcpy(process_stop_cmd,block->stop_cmd);
    //加入进程名称
    ZVAL_LONG(&process_pid,block->pid);
    //退出码
    ZVAL_LONG(&process_exit_code,block->exit_code);
    //信号中断码
    ZVAL_LONG(&process_sig_no,block->sig_no);
    //当前运行状态
    ZVAL_LONG(&process_run_state,block->state);
    //内存池索引
    ZVAL_LONG(&process_index,block->index);
    add_assoc_string(process_array,SFF_PROCESS_NAME,process_name_back);
    add_assoc_string(process_array,SFF_PROC_START_CMD,process_start_cmd);
    add_assoc_string(process_array,SFF_PROC_STOP_CMD,process_stop_cmd);
    //加入进程pid
    zend_hash_str_add(Z_ARRVAL_P(process_array),SFF_PID,strlen(SFF_PID),&process_pid);
    //退出码
    zend_hash_str_add(Z_ARRVAL_P(process_array),SFF_PROC_EXIT_CODE,strlen(SFF_PROC_EXIT_CODE),&process_exit_code);
    //信号终端码
    zend_hash_str_add(Z_ARRVAL_P(process_array),SFF_PROC_SIG_NO,strlen(SFF_PROC_SIG_NO),&process_sig_no);
    //运行状态
    zend_hash_str_add(Z_ARRVAL_P(process_array),SFF_PROC_RUN_STATE,strlen(SFF_PROC_RUN_STATE),&process_run_state);
    //进程索引
    zend_hash_str_add(Z_ARRVAL_P(process_array),SFF_PROC_INDEX,strlen(SFF_PROC_INDEX),&process_index);
    return SFF_TRUE;
}