//
// Created by zhanglei on 19-4-11.
//
#ifndef SFF_SSF_COMMON_H

#include "../sff_common.h"

#endif
//初始化日志库
void init_log_lib(){

    //初始化区域
    bzero(container_instance.log_lib, sizeof(sff_log));

    //写入日志
    container_instance.log_lib->write_log = sff_write;
}

int sff_write(char* pathname,char *content,size_t file_size,const char* mode)
{
    FILE* handel = fopen(pathname,mode);

    if(!handel)
    {
        close(fileno(handel));
        php_error_docref(NULL, E_WARNING, "write file error:%s\n",strerror(errno));
        return  SFF_FALSE;
    }

    ssize_t res = write(fileno(handel),content,file_size);
    if(res < 0)
    {
        close(fileno(handel));
        php_error_docref(NULL, E_WARNING, "write file error:%s\n",strerror(errno));
        return  SFF_FALSE;
    }

    close(fileno(handel));
    return  SFF_TRUE;
}