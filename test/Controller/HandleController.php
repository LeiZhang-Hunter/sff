<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-4-12
 * Time: 下午12:26
 */
namespace Controller;

use Container\ManageContainer;

class HandleController{

    //接收到从云端下发的指令进行远程操作
    public function recvData($data)
    {
        $command = $data["action"];
        $index = (int)$data["index"];//进程的索引编号
        var_dump($data);
        //获取进程信息
        if($command == "start")
        {
            ManageContainer::$container->start($index);
        }else if($command == "stop")
        {
            //停止
            ManageContainer::$container->stop($index);
        }else{
            //重启
            ManageContainer::$container->stop($index);
            sleep(0.1);
            ManageContainer::$container->start($index);
        }
    }


}