<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-4-12
 * Time: 上午11:41
 */
namespace Container;
use Controller\HandleController;

class ContainerTcpHandle{

    public $config;

    //buffer 防止粘包
    private $buffer;

    private $handleController;

    //进程启动时候的钩子
    public function processStartHook($process_info)
    {
        //将消息发送往云端让云端进行处理
        unset($process_info["start_cmd"]);
        unset($process_info["stop_cmd"]);
        $data = $process_info;

        //对数据进行加密
        $data["happen_time"] = time();
        $data["type"] = "process";
        $send_data = ManageContainer::$instance->encrypt->ssl_encrypt($data).$this->config["split"];
        //将启动动作上报到云端
        $res = ManageContainer::$container->report($send_data);
    }

    //进程关闭时候的钩子
    public function processStopHook($process_info)
    {
        unset($process_info["start_cmd"]);
        unset($process_info["stop_cmd"]);
        $data = $process_info;

        //对数据进行加密
        $data["happen_time"] = time();
        $data["type"] = "process";
        $send_data = ManageContainer::$instance->encrypt->ssl_encrypt($data).$this->config["split"];
        //将关闭的动作上报到云端
        $res = ManageContainer::$container->report($send_data);
    }

    //收到数据的钩子
    public function receiveHook($data)
    {
        var_dump($data);
        $config = ManageContainer::$instance->configInstance->getConfig();
        $deal_data = explode($config["split"],$data);
        $len = count($deal_data);//计算数据长度
        foreach ($deal_data as $key=>$value)
        {
            //底层做解密处理
            if(($encryptData = ManageContainer::$instance->encrypt->sll_decrypt($value)))
            {
                if(!$this->handleController) {
                    $this->handleController = new HandleController();
                }
                $this->handleController->recvData($encryptData);

            }else{
                //如果是最后一个那么就
                if($key == $len)
                {
                    if(strlen($this->buffer) > $config["max_buffer_len"]) {
                        $this->buffer = "";//清空buffer防止内存泄露
                    }else{
                        $this->buffer .= $value;
                    }
                }
            }
        }
    }

    //链接socket的钩子
    public function socketConnectHook()
    {
        //链接上socket之后将公司信息和物理网卡发送到云端进行验证,如果验证成功那么就可以建立长连接
        $enterprise_uuid = isset($this->config["enterprise_uuid"]) ? $this->config["enterprise_uuid"] : "";
        $password = isset($this->config["password"]) ? $this->config["password"] : "";

        $data = [];
        $data["enterprise_uuid"] = $enterprise_uuid;
        $data["password"] = md5(ManageContainer::$instance->encrypt->ssl_encrypt($password));
        $data["type"] = "process";
        $encry_data = ManageContainer::$instance->encrypt->ssl_encrypt($data).$this->config["split"];
        //发送验证
        $res = ManageContainer::$container->report($encry_data);
    }

    //关闭socket时候触发的钩子
    public function socketCloseHook()
    {

    }
}