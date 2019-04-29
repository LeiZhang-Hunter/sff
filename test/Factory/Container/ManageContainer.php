<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-4-9
 * Time: 下午6:57
 */
namespace Container;

use Controller\HandleController;

class ManageContainer{

    //配置
    public $config;

    //容器的实例
    public static $container;

    /**
     * @var ManageContainer
     */
    public static $instance;

    //运行的钩子
    private $runHook;

    //配置的实例
    /**
     * @var ContainerConfig
     */
    public $configInstance;

    /**
     * @var ContainerTcpHandle
     */
    public $handleHook;

    /**
     * @var \Encrypt
     */
    public $encrypt;

    private $pid;

    public function __construct()
    {
        self::$instance = $this;
    }

    //创建容器的实例
    public function create()
    {
        self::$container = new \SffContainer();
    }

    public function loadInstance($name,$instance)
    {
        //加载实例
        if(is_object($instance)) {
            $this->$name = $instance;
        }else{
            if(is_array($instance))
            {
                if(is_file($instance[1])) {
                    include_once $instance[1];
                    $this->$name = new $instance[0];
                }else{
                    throw new \Exception("loadInstance dir error");
                }
            }else{
                throw new \Exception("loadInstance param error");
            }
        }
    }

    //启动时候的钩子
    public function loadRunHook($hook)
    {
        $this->runHook = $hook;
    }

    //获取容器的pid
    public function getContainerPid()
    {
        if(!$this->pid)
        {
            if(is_file(BASE_DIR."/Pid/server.pid")) {
                $this->pid = file_get_contents(BASE_DIR."/Pid/server.pid");
            }else{
                $this->pid = 0;
            }
        }
        return $this->pid;
    }

    //运行容器
    public function run()
    {
        //检查启动参数
        global $argv;

        call_user_func($this->runHook,$this);

        //将配置注入到里面
        $this->handleHook->config = $this->config;

        //注册链接上云端的时候的钩子
        self::$container->socketConnectHook([$this->handleHook,"socketConnectHook"]);

        //注册socket关闭时候的钩子
        self::$container->socketCloseHook([$this->handleHook,"socketCloseHook"]);

        //注册进程启动时候的钩子
        self::$container->processStartHook([$this->handleHook,"processStartHook"]);

        //注册进程关闭时候的钩子
        self::$container->processStopHook([$this->handleHook,"processStopHook"]);

        //注册进程收到数据时候的钩子
        self::$container->receiveHook([$this->handleHook,"receiveHook"]);

        //运行
        self::$container->run();
    }

}