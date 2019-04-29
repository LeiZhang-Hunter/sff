<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-4-12
 * Time: 下午12:32
 */
class Encrypt{

    private $key;

    public function setKey($key)
    {
        $this->key = $key;
    }


    public function ssl_encrypt($data)
    {
        if (!$data) {
            return false;
        }
        $encrypt = [];
        $key = $this->key;//固定形成的加密钥匙为了防止别人容易破解采用md5混淆,与解密处必须对应
        $encrypt['data'] = json_encode($data);//要加密的数据
        $encrypt['iv'] = base64_encode($this->key);//另一处对应的值,与解密处必须对应
        $encrypt['value'] = (openssl_encrypt($encrypt['data'], 'AES-128-CBC', $key, 0, $encrypt['iv']));
        return isset($encrypt["value"]) ? $encrypt["value"] : "";
    }

    /**
     * Description: openssl解密
     * @param $encrypt
     * @return bool|mixed
     */
    public function sll_decrypt($encrypt)
    {
        if (!$encrypt) {
            return false;
        }
        $key = $this->key;//固定形成的加密钥匙为了防止别人容易破解采用md5混淆,与加密处必须对应
        $decrypt = openssl_decrypt(($encrypt), 'AES-128-CBC', $key, 0, trim(base64_encode($this->key)));
        return json_decode($decrypt,1);
    }

}