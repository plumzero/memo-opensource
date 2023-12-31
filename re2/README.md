
### 测试版本

- 标签为 `2020-11-01`


### 安装步骤

克隆仓库后采用 `out-of-source` cmake 编译安装。

可以生成动态库供调用，但缺省时不会生成动态库，所以这里需要指定。
```cmake
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local/re2 -DBUILD_SHARED_LIBS=on ..

    make

    make install
```


### 名词解释

为了方便叙述，引入一些名词进行说明:

| 名词 | 说明 |
|:---:|:-----|
| 模式匹配规则 | 正则表达式字符串 |
| 正则表达式对象 | 可以理解为对象化的模式匹配规则字符串，程序中某些情况下可以与模式匹配规则互用 |
| 模式串 | 即模式匹配规则 |
| 正则集 | 正则表达式对象的集合，这里是一个按时间顺序添加进来的数组 |
| 目标串 | 用于匹配的普通字符串 |
| 匹配子串 | 目标串中符合模式匹配规则的部分字符串 |


### 使用说明

re2 支持 PCRE 语法，适用于基本正则与扩展正则。


### 接口

[类接口(较不实用)](doc/FilteredRE2.md)

[全局接口(较实用)](doc/Global.md)
