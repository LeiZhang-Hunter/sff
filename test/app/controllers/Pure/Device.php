<?php

namespace Pure;

use Model\ShopModel;
use Model\ShopTestModel;

class Device extends \FactoryController{
    public function addFan()
    {


        $shopModel = new ShopModel();
        var_dump($shopModel);die;
        var_dump(\SuperAutoLoader::$namespace_store);die;

        return 1;
    }

    public function __destruct()
    {

    }
}