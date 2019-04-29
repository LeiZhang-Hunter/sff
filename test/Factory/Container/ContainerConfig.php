<?php
/**
 * Description:加载容器配置
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-4-9
 * Time: 下午7:00
 */
namespace Container;

use mysql_xdevapi\Exception;

class ContainerConfig {

    private $sffloader;

    //容器的配置
    private $container_config;

    //实例化配置
    public function __construct()
    {
        //实例化容器配置加载类
        $this->sffloader = new \Config();

        //进行配置初始化
        if(!$this->sffloader->initConfig(BASE_DIR."/Config/Container"))
        {
            //如果说没有初始化成功
            throw new Exception("load container config error");
        }
    }

    //获取容器的配置
    public function getConfig($key = '')
    {
        //如果说没有加载过配置选项
        if(!$this->container_config) {
            $config = $this->sffloader->getConfig();

            if(isset($config[ENV]["Container"]))
            {
                $this->container_config = $config[ENV]["Container"];
            }else{
                $this->container_config = [];
            }
        }

        if(!$key) {
            return $this->container_config;
        }else{
            return isset($this->container_config[$key]) ? $this->container_config[$key] : false;
        }
    }

}