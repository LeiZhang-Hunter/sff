<?php

$sff = new SffFactoryController();
$a = $sff->createMem();



$pid = pcntl_fork();
if($pid == 0)
{
    sleep(1);
    var_dump($a);
}else{

    $a["aaa"] = 1;
    var_dump($a);
}