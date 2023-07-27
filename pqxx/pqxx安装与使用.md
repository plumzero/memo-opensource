
#### 环境与版本

Debian 8.0

postgresql-13.1

gcc version 4.9.2

libpqxx-5.1

libpqxx-6.X 已经开始支持 C++14 了，libpqxx-7.X 已经开始支持 C++17 了，所以这里选择 libpqxx-5.X 版本。


#### 安装

依赖安装
```sh
    apt-get install libpq-dev
```

此外，还需要将 python 版本设置为 3 。

解压 配置
```sh
    export PG_CONFIG=/usr/local/pgsql/bin/pg_config
    
    unzip libpqxx-5.1.zip
    cd libpqxx-5.1
    ./configure --prefix=/usr/local/pqxx --disable-documentation --enable-shared
```
因为 C++ 的名称修饰问题，这里建议编译成动态库形式。


编译 安装
```sh
    make
    make install
```
