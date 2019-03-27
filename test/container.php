<?php

$container = new SffContainer();
$container->setConfig([
    "user"=>"root",
    "daemon"=>false,
    "container_ip"=>"127.0.0.1",
    "container_port"=>9000,
    "process_pool"=>[
        "test1"=>[],
        "test2"=>[],
        "test3"=>[],
        "test4"=>[],
        "test5"=>[]
    ]
]);
$a = $container->run();
