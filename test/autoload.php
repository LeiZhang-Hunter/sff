<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-4-9
 * Time: 下午6:34
 */

//定义运行环境
define("ENV","develop");

//进行自动加载
define("__ROOT__",__DIR__."/../..");

define("BASE_DIR",__DIR__);


//开启文件导入
SuperAutoLoader::addNameSpace("Container",BASE_DIR."/Factory/Container");

SuperAutoLoader::addNameSpace("Controller",BASE_DIR."/Controller");


//开启加载监控
SuperAutoLoader::start_monitoring();


$container = new \Container\ManageContainer();

//创建一个管理容器
$container->create();


//载入配置实例
$container->loadInstance("configInstance",new \Container\ContainerConfig());
$container->loadInstance("handleHook",new \Container\ContainerTcpHandle());
$container->loadInstance("encrypt",["Encrypt",BASE_DIR."/Class/Encrypt.php"]);