//
// Created by zhanglei on 19-3-22.
//

#ifndef SFF_SFF_SIGNAL_HANDLE_H
#define SFF_SFF_SIGNAL_HANDLE_H

#endif //SFF_SFF_SIGNAL_HANDLE_H

#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"
#endif

#define SIG_POOL 64

typedef struct _sff_signal_handle
{
    //信号处理池
    void* (*signal_pool)[SIG_POOL];

    SFF_BOOL (*add_signal_handle)(int __sig, __sighandler_t __handler);
}sff_signal_handle;

//初始化信号处理器
SFF_BOOL init_signal_handle(sff_signal_handle* handle);
//添加信号处理函数
SFF_BOOL add_signal_handle(int __sig, __sighandler_t __handler);