
记录两个小问题，但却花了近 3 个小时... 回想起来，却是不应该的。故记录一下，以作警示!

> Question 1:

问题描述: undefined reference to `pqxx::tuple::operator[](int) const'

编译的时候一直报这个未定义，但是看了一下源文件，是定义过的。

去网上找，也没有关于这样的提问。

去 github 上找，发现也没这样的 issue 。

真的很奇怪...

**后来把报错的地方先注释掉，编译成源文件后，再 ldd 发现引用库路径错误。系统的库版本较低，没有上面这个实现。**

那就把系统的这个库删掉吧!


> Question 2:

问题描述: fatal error: pqxx/pqxx: No such file or directory

去查看 CMakeFiles 中生成的 Make 相关文件，是没有的。但实际上机器里面是有这个文件的。

原来是 CMake 文件写错了
```cmake
    include_directories(pgsql_INCLUDE)
    include_directories(pq_INCLUDE)
```

改成下面这样就好了
```cmake
    include_directories(${pgsql_INCLUDE})
    include_directories(${pq_INCLUDE})
```

忘记写点东西是难免的，但 cmake 不提示一下也太不地道了...
