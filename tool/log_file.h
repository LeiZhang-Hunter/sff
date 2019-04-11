//
// Created by zhanglei on 19-4-11.
//

#ifndef SFF_LOG_FILE_H
#define SFF_LOG_FILE_H

#endif //SFF_LOG_FILE_H

#ifndef SFF_SSF_COMMON_H

#include "../sff_common.h"

#endif

typedef struct _sff_log{

    //写入日志
    int (*write_log)(char* pathname,char *content,size_t file_size);

}sff_log;

//初始化日志库
void init_log_lib();

int sff_write(char* pathname,char *content,size_t file_size);


