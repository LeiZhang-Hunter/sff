<?php

class A{
    public static function test($data)
    {
            var_dump($data);
    }
}

$in = new A();
$start = memory_get_usage();
$container = new SffContainer();
$container->setConfig([
    "user"=>"root",
    "daemon"=>false,
    "container_ip"=>"127.0.0.1",
    "container_port"=>9001,
    "process_pool"=>[
        "swoole_fan"=>[
            "start"=>"php /home/zhanglei/data/www/pureliving/swoole_fan/swoole_fan.php",
            "stop"=>""
        ],
    ]
]);

$container->processStartHook(['A','test']);
$container->processStopHook(['A','test']);
$container->receiveHook(['A','test']);
$a = $container->run(true);
$end = memory_get_usage();
//echo $end-$start."\n";
