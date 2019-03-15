<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-3-4
 * Time: 下午3:06
 */
namespace Model;


class ShopTestModel {

    protected $table = "shop";

    public function __construct()
    {
    }

    //获取商家推荐列表
    public function getRecommendList()
    {
        var_dump(\SimSuperFactory::$sff_config);die;
//        var_dump($a);
    }
}