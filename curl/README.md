

使用版本: curl-7.75.0

openssl 版本: 1.1.1j


### 编译安装

解压
```sh
    tar -zxvf curl-7.75.0.tar.gz
    cd curl-7.75.0
```

有说在编译之前执行 `autoreconf -fi` 生成适应于当前系统的 `configure` 文件。不置可否，如果编译的库也要在其他 Linux 上使用的话，我觉得可以使用默认的 `configure` 文件。

配置 && 编译 && 安装
```sh
    ./configure --prefix=/usr/local/curl --without-ssl --enable-shared
    make
    make install
```


