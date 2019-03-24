<?php

$container = new SffContainer();
$container->setConfig([
    "user"=>"root",
    "daemon"=>false,
    "container_ip"=>"127.0.0.1",
    "container_port"=>9000
]);
$a = $container->run();
