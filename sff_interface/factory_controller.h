//
// Created by zhanglei on 19-2-23.
//

#ifndef SFF_SUPER_FACTORY_H
#define SFF_SUPER_FACTORY_H

#endif //SFF_SUPER_FACTORY_H
//在栈上申请内存
zend_class_entry factory_controller;

//栈上申请内存的实例 存储为一个全局变量
zend_class_entry *factory_controller_entry;


//定义工厂控制器的构造函数
PHP_METHOD (SffFactoryController, __construct);

//获取类的实例
PHP_METHOD(SffFactoryController,getFactoryInstance);

//创建共享内存
PHP_METHOD(SffFactoryController,createMem);

//工厂的析构函数
PHP_METHOD (SffFactoryController, __destruct);


//定义主类的实例
#ifndef SFF_APPLICATION_INSTANCE
#define SFF_APPLICATION_INSTANCE "framework_instance"
#endif

//定义自己的静态实例
#ifndef FACTORY_INSTANCE
#define FACTORY_INSTANCE "_factory_instance"
#endif

void factory_application_init();