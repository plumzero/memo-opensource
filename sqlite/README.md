
### 安装环境参考

- version 3.31.1
- [sqlite-autoconf-3310100.tar.gz](https://www.sqlite.org/download.html)
- 安装环境: VM + RHEL7.5
- 注意: 安装包不要直接从 github 源上下载，因为可能会出现类似下面的错误:
  ```shell
    make: *** No rule to make target `/usr/local/sqlite-version-3.31.1/manifest', needed by `sqlite3.h'.  Stop.
  ```

### 编译安装

- 解压
  ```shell
    tar -zxvf sqlite-autoconf-3310100.tar.gz
  ```

- 配置
  ```shell
    cd sqlite-autoconf-3310100
    ./configure --prefix=/usr/local/sqlite --disable-tcl
  ```

- 编译及安装
  ```shell
    make
    make install
  ```

- 测试程序文件执行顺序
    + sqlite_connect.cpp
    + sqlite_create.cpp
    + sqlite_insert.cpp
    + sqlite_select.cpp
    + sqlite_update.cpp
  
### C/C++ 接口 API

- `sqlite3_open(const char *filename, sqlite3 **ppDb)`
    + 该例程打开一个指向 SQLite 数据库文件的连接，返回一个用于其他 SQLite 程序的数据库连接对象。
    + 如果 filename 参数是 NULL 或 ':memory:'，那么 sqlite3_open() 将会在 RAM 中创建一个内存数据库，这只会在 session 的有效时间内持续。
    + 如果文件名 filename 不为 NULL，那么 sqlite3_open() 将使用这个参数值尝试打开数据库文件。如果该名称的文件不存在，sqlite3_open() 将创建一个新的命名为该名称的数据库文件并打开。
- `sqlite3_exec(sqlite3*, const char *sql, sqlite_callback, void *data, char **errmsg)`
    + 该例程提供了一个执行 SQL 命令的快捷方式，SQL 命令由 sql 参数提供，可以由多个 SQL 命令组成。
    + 在这里，第一个参数 sqlite3 是打开的数据库对象，sqlite_callback 是一个回调，data 作为其第一个参数，errmsg 将被返回用来获取程序生成的任何错误。
    + sqlite3_exec() 程序解析并执行由 sql 参数所给的每个命令，直到字符串结束或者遇到错误为止。
- `sqlite3_close(sqlite3*)`
    + 该例程关闭之前调用 sqlite3_open() 打开的数据库连接。所有与连接相关的语句都应在连接关闭之前完成。
    + 如果还有查询没有完成，sqlite3_close() 将返回 SQLITE_BUSY 禁止关闭的错误消息。

[测试程序](t)
