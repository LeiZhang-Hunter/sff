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
                                                    "swoole_fan"=>[
                                                        "start"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/swoole_fan.php start",
                                                        "stop"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/swoole_fan.php stop"
                                                    ],
                                                    "auto"=>[
                                                        "start"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/auto.php start",
                                                        "stop"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/auto.php stop"
                                                    ],
                                                    "client"=>[
                                                        "start"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/client.php start",
                                                        "stop"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/client.php stop"
                                                    ],
                                                    "indoor"=>[
                                                        "start"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/indoor.php start",
                                                        "stop"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/indoor.php stop",
                                                    ]
                                                ],
                          "split"=>"\r\n\r\n",
                          "pidfile"=>"/home/zhanglei/data/www/pureliving/new_client/Pid/server.pid",
                          "encrypt_key"=>"111111111111",
                          "max_buffer_len"=>"6144"
                      ]);

$container->processStartHook(['A','test']);
$container->processStopHook(['A','test']);
$container->receiveHook(['A','test']);
$a = $container->run(true);
$end = memory_get_usage();
//echo $end-$start."\n";
