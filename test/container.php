<?php

$container = new SffContainer();
$container->setConfig([
    "user"=>"root",
    "daemon"=>false,
    "container_ip"=>"127.0.0.1",
    "container_port"=>9000,
    "process_pool"=>[
        "test1"=>[
            "start"=>"start1",
            "stop"=>"stop133",
        ],
        "test2"=>[
            "start"=>"start2",
            "stop"=>"stop2",
        ],
        "test3"=>[
            "start"=>"start3",
            "stop"=>"stop3",
        ],
        "test4"=>[
            "start"=>"start4",
            "stop"=>"stop44",
        ],
        "test5"=>[
            "start"=>"start5",
            "stop"=>"stop5",
        ]
    ]
]);
$a = $container->run();
