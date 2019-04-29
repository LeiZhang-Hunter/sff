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
    "connect_server"=>true,
    "container_ip"=>"127.0.0.1",
    "container_port"=>8723,
    "process_pool"=>[
        "test1"=>[
            "pid_file"=>BASE_DIR."/Pid/test1.pid",
            "start"=>"php ".BASE_DIR."/Process/test1.php",
            "stop"=>"php test1.php stop"
        ],
        "test2"=>[
            "pid_file"=>BASE_DIR."/Pid/test2.pid",
            "start"=>"php ".BASE_DIR."/Process/test2.php",
            "stop"=>"php test2.php stop"
        ],
        "test3"=>[
            "pid_file"=>BASE_DIR."/Pid/test3.pid",
            "start"=>"php ".BASE_DIR."/Process/test3.php",
            "stop"=>"php test3.php stop"
        ],
    ],
    "split"=>"\r\n\r\n",
    "pidfile"=>BASE_DIR."/Pid/server.pid",
    "encrypt_key"=>"111111111111",
    "max_buffer_len"=>"6144",
    "enterprise_uuid"=>"0A2360D174B6EAAD10BE997215A82ED4",
    "password"=>"0A2360D174B6EAAD10BE997215A82ED4",
    //容器需要设置的socket接收buf
    "container_recv_buff"=>"",
    //容器需要设置的socket发送buf
    "container_send_buff"=>"",
];
