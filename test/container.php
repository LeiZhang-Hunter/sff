<?php

$container = new SffContainer();
$container->setConfig([
    "user"=>"root",
    "daemon"=>true,
]);
$container->run();
