<?php

class A{
    public static function test($data)
    {
        var_dump(11);
    $this->container->report("ahahahah");
            var_dump($data);
    }
}

$in = new A();
$start = memory_get_usage();
$container = new SffContainer();
$in->container = $container;
$container->setConfig([
                          "user"=>"root",
                          "daemon"=>false,
                           "connect_server"=>true,
                          "container_ip"=>"127.0.0.1",
                          "container_port"=>8723,
                          "process_pool"=>[
                                                    "swoole_fan"=>[
                                                        "pid_file"=>"/home/zhanglei/data/www/pureliving/new_client/Pid/swoole_fan.pid",
                                                        "start"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/swoole_fan.php start",
                                                        "stop"=>"php /home/zhanglei/data/www/pureliving/new_client/Factory/Process/swoole_fan.php stop"
                                                    ]],
                          "split"=>"\r\n\r\n",
                          "pidfile"=>"/home/zhanglei/data/www/pureliving/new_client/Pid/server.pid",
                          "encrypt_key"=>"111111111111",
                          "max_buffer_len"=>"6144"
                      ]);

$container->socketConnectHook(['A','test']);
$a = $container->run(true);
$end = memory_get_usage();
//echo $end-$start."\n";
