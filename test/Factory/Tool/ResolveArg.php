<?php
/**
 * 这是一个命令行参数接卸的类，由于需要配合老的进程包裹，同时兼容新的sff容器中的自动文件加载机制，所以需要带着命名空间，所以不要疑惑
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-4-16
 * Time: 上午10:14
 */
namespace Tool;
class ResolveArg
{
    private static $pid = 0;

    private static $beforeStopHook;

    public static function loadPid($pid)
    {
        self::$pid = $pid;
    }

    //停止之前的回调钩子
    public static function stopBeforeHook($hook)
    {
        self::$beforeStopHook = $hook;
    }

    //解析参数
    public static function parseArg($process_pid_file_name)
    {
        global $argv;

        $command = isset($argv[1]) ? $argv[1] : "";
        if($command != "stop" && $command!="start" && $command != "reload")
        {
            exit("please input stop|start|reload\n");
        }

        $pid = self::getPid($process_pid_file_name);
        if($command == "stop")
        {
            if(!$pid)
            {
                exit("process not run");
            }else{

                //触发下线前置钩子
                if(is_callable(self::$beforeStopHook)) {
                    call_user_func(self::$beforeStopHook);
                }

                //发送关闭信号
                posix_kill($pid,SIGTERM);
                exit();
            }
        }else if($command == "start")
        {
            if($pid)
            {
                exit("process have run");
            }

        }else{
            if(!$pid)
            {
                exit("process not run");
            }else{
                //发送关闭信号
                posix_kill($pid,SIGUSR1);
            }

        }
    }

    //生成配置文件
    public static function makePid($process_pid_file_name,$pid)
    {
        if (!$pid) {
            unlink(config("Config")[$process_pid_file_name]);
        } else {
            file_put_contents(config("Config")[$process_pid_file_name], $pid);
        }
    }

    public static function getPid($process_pid_file_name)
    {
        if(is_file(config("Config")[$process_pid_file_name])) {
            return file_get_contents(config("Config")[$process_pid_file_name]);
        }else{
            return 0;
        }
    }
}