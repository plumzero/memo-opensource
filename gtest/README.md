
测试版本: 1.10.0

### 编译安装

进入顶层目录下，进行 out-of-source 编译，安装到 `/usr/local/gtest` 目录下
```sh
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local/gtest -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=on
    make -j 4
    make install
```
