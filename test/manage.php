<?php
/**
 * 此程序借鉴过swoole framework 的 RPC程序
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-4-9
 * Time: 下午6:29
 */

include_once "autoload.php";


//加载运行的钩子
$container->loadRunHook(function ($instance){
    $config = $instance->configInstance->getConfig();
    $instance->config = $config;
    \Container\ManageContainer::$container->setConfig($config);

});

$container->encrypt->setKey($container->configInstance->getConfig("encrypt_key"));

$container->run();