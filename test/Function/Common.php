<?php
/**
 * Created by PhpStorm.
 * User: Abel
 * Date: 2018-6-18 0018
 * Time: 19:28
 */

/**
 * @description 获取配置信息
 * @param $item
 * @return mixed|null
 */
function config($item)
{
    static $config;
    if(!isset($config[$item]))
    {
        $get_config = include_once __ROOT__."/Config/Process/{$item}.php";
        if($get_config) {
            $config[$item] = $get_config;
            return $get_config;
        }else{
            include_once __ROOT__."/Config/Process/{$item}.php";
            return NULL;
        }
    }else{
        return $config[$item];
    }
}
//
function writeLog($msg)
{

    $file_path = __ROOT__."/Log/".date("Y-m-d",time()).".log";
    $fp = fopen($file_path,"a+");
    if (flock($fp, LOCK_EX)) {  // 进行排它型锁定
        $writeData = date("Y-m-d H:i:s",time())." : ".$msg."\r";
        fwrite($fp, "$writeData\n");
        flock($fp, LOCK_UN);    // 释放锁定
        fclose($fp);
        return true;
    } else {
        fclose($fp);
        return false;
    }
}

function sendLog($msg)
{
//    $ch = curl_init(config("Config")["api_url"]."/OutApi/Receive/insertLog");
//    curl_setopt($ch,CURLOPT_RETURNTRANSFER,true);
//    curl_setopt($ch,CURLOPT_SSL_VERIFYPEER,false);
//    curl_setopt($ch,CURLOPT_POSTFIELDS,[
//        "content"=>config("Config")["enterprise_uuid"].":".$msg,
//    ]);
//    $result = curl_exec($ch);
//    curl_close($ch);
//    $jsonArr = false;
//    if($result)
//    {
//        $jsonArr = json_decode($result,1);
//        if(isset($jsonArr['errcode']) && $jsonArr['errcode'] == 0)
//        {
//            if($jsonArr['errcode'] == 0) {
//                $jsonArr = true;
//            }else{
//                $jsonArr = false;
//                writeLog($jsonArr["errmsg"]);
//            }
//        }else{
//            $jsonArr = false;
//        }
//    };
//    return $jsonArr ? true : false;
}

//将16进制转化为数组
function covertHexToArray($hex)
{
    $struct = [];
    $count = strlen($hex);
    for ($i=0;$i<$count;$i++)
    {
        $struct[] = $hex[$i];
    }
    return $struct;
}

//过滤信息
function filterGetInfo($data,$key,$defaultValue="")
{
    return isset($data[$key]) ? $data[$key] : $defaultValue;
}

//打印16进制字符串
function printHex($str){
    for ($i = 0; $i < strlen($str); $i++) {
        printf("0x%x ", ord($str[$i]));
    }
    printf("\n");
    return;
}