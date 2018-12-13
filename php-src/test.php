<?php
include("LSYS/SimHash.php");
include("LSYS/SimHashFactory.php");
$a=new LSYS\SimHashFactory(32);
$assss=["你好","bbb","ccc"];
$assss1=["你好","bbb","ccc"];
//$a->setWeight(['ccc'=>2]);//设置某个关键字权重,默认为1
$b=new LSYS\SimHash($a->fingerprint($assss));
$c=new LSYS\SimHash($a->fingerprint($assss1));
//16进制输出
//var_dump($b->gethex());
//var_dump($c->gethex());
//2进制输出
//var_dump($b->getbin());
//var_dump($c->getbin());
//10进制输出
//var_dump($b->getdec());
//var_dump($c->getdec());
//海明距离
var_dump($b->hammingDistance($c));
//相似度
var_dump($b->compareWith($c));
