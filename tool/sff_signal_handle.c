//
// Created by zhanglei on 19-3-22.
//

#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"
#endif

SFF_BOOL init_signal_handle(sff_signal_handle* handle)
{
    bzero(handle, sizeof(sff_signal_handle));

    handle->add_signal_handle = add_signal_handle;
}

SFF_BOOL add_signal_handle(int __sig, __sighandler_t __handler)
{

}