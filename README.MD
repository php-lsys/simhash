文本相似度比较(simhash算法)
===

> PHP扩展方式实现

> php-src 为PHP源码方式实现

#### linux or mac 下编译

```shell
cd simhash
phpize && ./configure
make && make install
```

#### windows 下编译

> load build env 

```
打开 [x64|x86] Native Tools Command Prompt for VS 2017 编译命令行
加载PHP编译变量 \your-php-sdk-binary-tools-dir\bin\phpsdk_setvars.bat 
```

```cmd
cd your_php_src # PHP 源码目录 下载 simhash 源码到 ext/simhash 下
buildconf.bat
configure --disable-all --enable-cli --enable-simhash=shared 
nmake
```
