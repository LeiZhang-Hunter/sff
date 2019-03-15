<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-3-4
 * Time: 下午1:34
 */
//实例化框架全局句柄
define("__ROOT__",__DIR__);


define("ENV","test");



$start = memory_get_usage();

//初始化主类，进行路由派遣
$handle = new SimSuperFactory(__DIR__);

//设置框架主目录
$handle->setMain("app");

//注册运行前的钩子
$handle->onRequestBefore(function($builder){


    //加入命名空间--
SuperAutoLoader::addNameSpace("Api",__ROOT__."/app/controllers/Api");
SuperAutoLoader::addNameSpace("System",__ROOT__."/system");
SuperAutoLoader::addNameSpace("TraitLib",__ROOT__."/system/TraitLib");
SuperAutoLoader::addNameSpace("Model",__ROOT__."/app/models");
SuperAutoLoader::start_monitoring();



    //实例化配置
    $configBuilder = new Config();

    //初始化配置加载项
    $result = $configBuilder->initConfig();

   //注入配置
    if($result)
    {
        //获取配置选项
        $config = $configBuilder->getConfig();
        var_dump($config);die;
           die;

        if(ENV == "test")
        {
            $productConfig = $config["test"];
        }else{
            $productConfig = $config["product"];
        }
        //将初始化配置加载项放入php的全局变量里面
        $builder->setConfig($productConfig);
    }

});
//执行路由
$handle->run();



$end = memory_get_usage();
echo $end-$start."\n";