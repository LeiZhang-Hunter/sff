<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-4-9
 * Time: 下午7:00
 */

return [
    "user"=>"root",
    "daemon"=>false,
    "container_ip"=>"127.0.0.1",
    "container_port"=>9001,
    "process_pool"=>[
        "swoole_fan"=>[
            "start"=>"php /home/zhanglei/data/www/pureliving/swoole_fan/swoole_fan.php",
        ],
    ],
    "split"=>"\r\n\r\n"
];