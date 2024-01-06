
### 版本

- sqlite: 3.41.0
- sqlite_orm: 1.6

采用源文件测试方式。

注意: sqlite_orm 需要 C++14 支持，不过部分 C++14 特性可能在较低版本 g++ 编译器上有实现，所以需要注意不要使用太高的 sqlite_orm 版本，否则可能编译不过。经测试，这里选择 1.6。

[sqlite历史版本链接](https://www.sqlite.org/cgi/src/taglist)


### 使用注意

- CRUD 方法 `get`, `get_pointer`, `remove`, `update`(不是 `insert`) 只有在有主键时才能使用。


### 示例

- [查询示例](02_second/select.cpp)
- [插入示例](02_second/insert.cpp)
- [更新示例](02_second/update.cpp)
- [删除示例](02_second/delete.cpp)

- [查询示例](02_second/students.cpp)