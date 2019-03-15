<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-3-4
 * Time: 下午1:43
 */

namespace Api;


class Index extends \FactoryController{

    public function home()
    {
        echo 11;die;

        $shopModel = new ShopModel();
        $loader = new SuperAutoLoader();
        $loader->addNameSpace("Api",__ROOT__."/app/controllers/Api");
        $loader->addNameSpace("System",__ROOT__."/system");
        $loader->addNameSpace("TraitLib",__ROOT__."/system/TraitLib");
        $loader->addNameSpace("Model",__ROOT__."/app/models");
        //获取商家推荐列表
        $shopModel->getRecommendList();
    }

}