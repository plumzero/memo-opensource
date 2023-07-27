
- 测试一下 mimalloc 内存分配器的效率。一共测试两个版本，分别是 `v2.0.6` 和 `v1.7.6` 。
- 编译生成 mimalloc 库时对 cmake 版本要求较高，这里使用 3.15.0 。

进入项目下编译、安装:
```sh
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local/mimalloc ..
    make
    make install
```

顺利的话会在 `/usr/local/mimalloc` 目录下生成相关的头文件和库文件。



测试小结: 效率也没提高~
