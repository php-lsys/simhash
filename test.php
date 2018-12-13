<?php
$a=new LsExt\SimHashFactory(32);
//请先把文章进行分词,中文分词算法网上大把.英文直接用空格拆分即可
$assss=["你好","bbb","ccc"];
$assss1=["你好","bbb","ccc"];
//$a->setWeight(['ccc'=>2]);//设置某个关键字权重,默认为1
$b=new LsExt\SimHash($a->fingerprint($assss));
$c=new LsExt\SimHash($a->fingerprint($assss1));
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
?>
