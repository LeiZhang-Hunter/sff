//
// Created by zhanglei on 19-3-22.
//

#ifndef SFF_SSF_COMMON_H
#include "../sff_common.h"
#endif

SFF_BOOL init_signal_handle()
{
    bzero(container_instance.signal_factory, sizeof(sff_signal_handle));

    bzero(container_instance.signal_factory->signal_pool, sizeof(container_instance.signal_factory->signal_pool));

    container_instance.signal_factory->add_signal_handle = add_signal_handle;

    return SFF_TRUE;
}

SFF_BOOL add_signal_handle(int signo, __sighandler_t sighandler_fun)
{
    container_instance.signal_factory->signal_pool[signo] = sighandler_fun;

    //设置这个信号处理函数
//    signal(signo,sighandler_fun);
    struct sigaction act, act_g;

    act.sa_handler = sighandler_fun;

    act.sa_flags = 0;
    if (sigaction(signo, &act, &act_g) < 0)
    {
        return SFF_FALSE;
    }
    return SFF_TRUE;
}