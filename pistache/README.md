
## 更多参考
- [pistache](https://www.worldlink.com.cn/en/osdir/pistache.html)

## revert-236-master 编译安装
- 将 pistache-revert-236-master.zip 放至 /usr/local/src 目录下并解压
  ```shell
    unzip pistache-revert-236-master.zip
  ```
- 创建编译目录，并编译安装
  ```shell
    cd pistache-revert-236-master
    mkdir build
    cd build
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/pistache ..
    make
    make install
  ```
- 查看生成后的文件
    + 顺利的话，在 /usr/local/pistache 会生成两个文件夹 include, lib。
    + 目录树如下:
      ```shell
        .
        ├── include
        │   └── pistache
        │       ├── async.h
        │       ├── client.h
        │       ├── common.h
        │       ├── cookie.h
        │       ├── date.h
        │       ├── description.h
        │       ├── endpoint.h
        │       ├── flags.h
        │       ├── http_defs.h
        │       ├── http.h
        │       ├── http_header.h
        │       ├── http_headers.h
        │       ├── iterator_adapter.h
        │       ├── listener.h
        │       ├── mailbox.h
        │       ├── mime.h
        │       ├── net.h
        │       ├── optional.h
        │       ├── os.h
        │       ├── peer.h
        │       ├── prototype.h
        │       ├── reactor.h
        │       ├── route_bind.h
        │       ├── router.h
        │       ├── serializer
        │       │   └── rapidjson.h
        │       ├── stream.h
        │       ├── tcp.h
        │       ├── timer_pool.h
        │       ├── transport.h
        │       ├── typeid.h
        │       ├── version.h
        │       └── view.h
        └── lib
            ├── cmake
            │   └── pistache
            │       ├── PistacheConfig.cmake
            │       ├── PistacheTargets.cmake
            │       └── PistacheTargets-release.cmake
            ├── libpistache.a
            └── pkgconfig
                └── libpistache.pc
      ```

## 测试
- 测试程序位于**revert-236-master测试**目录下面，分别是 hello_server.cc restful_server.cc 和 
  restful_client.cc 。编译如下:
  ```shell
    g++ -g -O0 -std=c++11 hello_server.cc -I/usr/local/pistache/include     \
        /usr/local/pistache/lib/libpistache.a -lpthread -o hello_server

    g++ -g -O0 -std=c++11 restful_server.cc -I/usr/local/pistache/include   \
        /usr/local/pistache/lib/libpistache.a -lpthread -o restful_server

    g++ -g -O0 -std=c++11 restful_client.cc -I/usr/local/pistache/include   \
        /usr/local/pistache/lib/libpistache.a -lpthread -o restful_client
  ```
- 运行 hello_server 测试程序
    + 在一个 shell 会话中打开 hello_server
      ```shell
        # ./hello_server
        Now listening on http://0.0.0.0:9080
      ```
    + 在另一个 shell 会话中执行如下命令:
      ```shell
        curl http://127.0.0.1:9080
      ```
      顺利的话会返回: Hello World
- restful 两端会话测试
    + 在一个 shell 会话中打开服务端(绑定到 8090 端口)
      ```shell
        ./restful_server
      ```
    + 在另一个 shell 会话中打开客户端与服务端建立通信(向 8090 端口发送请求)
      ```shell
        ./restful_client
      ```
- revert-236-master 分支版本的 restful 服务端可以在一个进程内开启多个服务(多个端口)。同时客户
  端在实例化时也不再与服务端地址绑定，即一个客户端可以与不同的服务端通信。为了进行对比，这里
  也提供了服务端开启双端口，每个客户端分别向这两个端口发送请求的测试，测试程序位于
  **revert-236-master测试**目录下面，分别是 restful_server_d.cc 和 restful_client_d.cc ，编译
  方法和测试方法同上。
- 测试结果(单端口与双端口对比)

| 端口\客户端数\tps  |   1   |   5   |   10   |   15   |
|:------------------:|:-----:|:-----:|:------:|:------:|
|  单端口            | 1200  | 4852  |  5331  |  5287  |
|  双端口            | 1200  | 3926  |  5052  |  5129  |

客户端收到响应与客户端请求之比(单位: %):

| 端口\客户端数\比例 |   1   |   5   |   10   |   15   |
|:------------------:|:-----:|:-----:|:------:|:------:|
|   单端口           |  100  | 99.99 | 99.97  | 99.99  |
|   双端口           |  100  | 99.95 | 99.97  | 99.98  |

多个客户端可以通过脚本实现。
- 测试结论如下
    + 客户端每秒大概发送 1200 条请求，而服务端每秒大概可以处理 5000 条请求；
    + 单端口与双端口效率更高，且服务质量更好；

## rest-description 与 revert-236-master 说明
- 两者测试时均使用长连接(HTTP/1.1 KeepAlive)进行测试；
- 两者客户端均使用连接池机制，但实现方式不同，造成客户端性能有较大差异，具体如下:
    + r2m 连接池底层数据结构是哈希表映射(unordered_map)，其键值为 服务端ip地址+端口，一条连接
      对应于一个服务，也就是说服务端对客户端所有的处理都是在一条连接上完成的；
    + rd 连接池底层数据结构是数组(vector)，客户端可以通过多个连接与一个服务建立通信，所以相比
      处理更快一些。
- 两者实现的服务端处理请求效率基本相同，都是每秒 5000 条左右。