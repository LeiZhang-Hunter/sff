<?php
/**
 * Created by PhpStorm.
 * User: zhanglei
 * Date: 19-3-4
 * Time: 下午3:12
 */
namespace System;

use TraitLib\PdoDriver;

class FactoryModel
{
    use PdoDriver;

    const STATE_DELETED = -1;
    const STATE_DISABLED = 0;
    CONST STATE_ENABLED = 1;

    protected $table = '';
    protected $prikey = 'id';
    protected $strWhere = '';
    protected $strWhereLast = '';

    //加密发送数据防止别人抓包
    public function encryptSendData($data)
    {
        return base64_encode(encrypt($data));
    }

    public function __construct()
    {
        $this->table = config('DB')['prefix'].$this->table;
    }

    public function querySql($sql,$param)
    {
        return $this->query($sql,$param);
    }

    public function add(array $datas)
    {
        isset($datas['created_at']) or $datas['created_at'] = time();
        $datas['uuid'] = uuid();
        return $this->insert($this->table, $datas);
    }

    public function modify(array $datas)
    {
        isset($datas['updated_at']) or $datas['updated_at'] = time();
        return $this->update($this->table, $datas);
    }

    public function findById($id)
    {
        return $this->find($this->table, $this->prikey, $id);
    }

    public function findByKey($key, $value)
    {
        return $this->find($this->table, $key, $value);
    }

    public function search($param = [],$page = 0, $perPage = 10)
    {
        $this->strWhereLast = $this->strWhere;
        $limit = $page ? sprintf('%s,%s', ($page - 1) * $perPage, $perPage) : '';
        return $this->select($this->table, $param, $limit);
    }

    public function searchKeys($page = 0, $perPage = 10)
    {
        $this->strWhereLast = $this->strWhere;
        $limit = $page ? sprintf('%s,%s', ($page - 1) * $perPage, $perPage) : '';
        return $this->selectAuto($this->table, [$this->prikey], $limit);
    }

    public function searchCount($whereCache = false)
    {
        $whereCache and $this->strWhere = $this->strWhereLast;
        $count = $this->selectAuto($this->table, ['count(id)']);
        return $count ? $count : 0;
    }

    /**
     * 分页查询
     */
    public function pagedump()
    {
        return $this->selectAuto($this->table, ['count(id) total']);
    }

    public function getMaxSort()
    {
        return (int)$this->orderBy('sort', 'DESC')->selectAuto($this->table, ['sort'], 1);
    }


    public function remove(array $ids)
    {
        $ids = implode("','", $ids);
        $sql = "DELETE FROM $this->table WHERE $this->prikey IN ('$ids')";
        return $this->query($sql);
    }

    public function setState(array $ids, $state = self::STATE_ENABLED)
    {
        $ids = implode("','", $ids);
        $sql = "UPDATE $this->table SET state = $state WHERE $this->prikey IN ('$ids')";
        return $this->query($sql);
    }

    public function where($key, $value, $operator = '=')
    {
        if (in_array(strtolower($operator), ['=', '>', '<', '>=', '<=', '!=', '<>', 'like'])) {
            $this->strWhere and $this->strWhere .= ' AND ';
            $this->strWhere .= "`$key` $operator '$value'";
        } elseif (strtolower($operator) == 'in') {
            $inStr = implode('","', (array)$value);
            $this->strWhere and $this->strWhere .= ' AND ';
            $this->strWhere .= "`$key` $operator (\"$inStr\")";
        } else {
            throw new \Exception('operator not allowed');
        }
        return $this;
    }

    public function orwhere($key, $value, $operator = '=')
    {
        if (in_array(strtolower($operator), ['=', '>', '<', '>=', '<=', '!=', '<>', 'like'])) {
            $this->strWhere and $this->strWhere .= ' OR ';
            $this->strWhere .= "`$key` $operator '$value'";
        } elseif (strtolower($operator) == 'in') {
            $inStr = implode('","', (array)$value);
            $this->strWhere and $this->strWhere .= ' OR ';
            $this->strWhere .= "`$key` $operator (\"$inStr\")";
        } else {
            throw new \Exception('operator not allowed');
        }
        return $this;
    }

    public function groupBy($key, $order = 'ASC')
    {
        if (!in_array(strtoupper($order), ['ASC', 'DESC'])) {
            throw new \Exception('order not allowed');
        }
        $this->strGroupBy and $this->strGroupBy .= ' , ';
        $this->strGroupBy .= "`$key` $order";
        return $this;
    }

    public function orderBy($key, $order = 'ASC')
    {
        if (!in_array(strtoupper($order), ['ASC', 'DESC'])) {
            throw new \Exception('order not allowed');
        }
        $this->strOrderBy and $this->strOrderBy .= ' , ';
        $this->strOrderBy .= "`$key` $order";
        return $this;
    }
    public function getLastSql()
    {
        return $this->pdoStatement ? $this->pdoStatement->queryString : false;
    }

    public function getLastError()
    {
        return $this->pdoStatement ? $this->pdoStatement->errorInfo()[2] : false;
    }

    public function getAffectRows()
    {

        return $this->pdoStatement ? $this->pdoStatement->rowCount() : false;
    }

    public function getLastInsertId()
    {
        return $this->PdoDriver()->lastInsertId();
    }

    //查询一条完整 最好前面加条件
    public function getInfo($field=[])
    {
        if(!$field)
        {
            $field = '*';
        }else{
            $field = implode(',',$field);
        }
        $where = $this->strWhere ? " WHERE $this->strWhere" : '';
        $groupBy = $this->strGroupBy ? " GROUP BY $this->strGroupBy" : '';
        $this->strGroupBy = '';
        $orderBy = $this->strOrderBy ? " ORDER BY $this->strOrderBy" : '';
        $this->strOrderBy = '';
        $sql = "SELECT $field FROM `$this->table` {$where}{$groupBy}{$orderBy} LIMIT 1";
        $this->strWhere = '';
        $info = $this->fetchRow($sql,$this->paramWhere);
        return $info ? $info : [];
    }

    public function beginThing()
    {
        $this->transaction();
    }

    public function deleteData()
    {
        return $this->delete($this->table);
    }

    public function doBack()
    {
        $this->rollback();
    }

    public function doCommit()
    {
        $this->commit();
    }
}