<?php
$start = memory_get_usage();
$container = new SffContainer();
$container->setConfig([
    "user"=>"root",
    "daemon"=>false,
    "container_ip"=>"127.0.0.1",
    "container_port"=>9000,
    "process_pool"=>[
        "swoole_fan"=>[
            "start"=>"php /home/zhanglei/data/www/pureliving/swoole_fan/swoole_fan.php",
        ],
    ]
]);
$a = $container->run();
$end = memory_get_usage();
echo $end-$start."\n";