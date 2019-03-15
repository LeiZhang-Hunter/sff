//
// Created by root on 18-12-3.
//

#ifndef SFF_SFF_ROUTE_H
#define SFF_SFF_ROUTE_H

#endif //SFF_SFF_ROUTE_H

#define MAXBUF 255
#define METHODNAMEMAX 48

//sff route pack
typedef struct sff_route_pack_{

    //模块
     char *module;

    //控制器
     char *controller;

    //方法
     char *method;

    const char *root_dir;

    //对路由进行派遣
    SFF_BOOL (*dispatch)(struct sff_route_pack_ *handel);

    //设置根目录
    void (*set_root_dir)(struct sff_route_pack_ *handel,const char *root_dir);

    //执行路由
    SFF_BOOL (*run)();

    //执行销毁
    SFF_BOOL (*destroy)(struct sff_route_pack_ *handel);

}sff_route_pack;

//初始化路由的句柄
sff_route_pack* ssf_route_pack_init(sff_route_pack *handel);

//进行派遣
SFF_BOOL sff_route_dispatch(sff_route_pack *handel);



//对派遣后的路由进行执行
SFF_BOOL sff_route_run();

//销毁
SFF_BOOL destroy(sff_route_pack *handel);

//设置根目录
void sff_set_root_dir(sff_route_pack *handel,const char *root_dir);
