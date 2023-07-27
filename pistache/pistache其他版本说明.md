

## 版本说明
- Pistache 没有明确的版本说明，可以根据分支进行选择使用。
- master 分支现在已经不兼容 c++11, 需要使用 c++14 进行编译，环境要求较高。
    [Code doesn't compile anymore for C++11](https://github.com/oktal/pistache/issues/634)
- rest-description 分支较老，测试时能够满足性能要求，但:
    + 该版本源码有些错误，导致 http client通过 body 传递参数时会引起程序崩溃，部分错误程序
      如下:
      ```c++
        // client.cc
        RequestBuilder&
        RequestBuilder::body(std::string val) {
            request_.body_ = std::move(val);
        }
        // ...
        Client::Options&
        Client::Options::keepAlive(bool val) {
            keepAlive_ = val;
        }
      ```
    + 比较发现该版本 client 属于试验版(Experimental)；
    + 头文件组织方式与 master 分支有较大差异；
    + 源文件，主要是名字空间与 master 分支有较大差异；
  非常不推荐使用。
 - io-reactor 分支也较老，http client 通过 body 传递参数成功，但测试大量请求时程序极容易随
   机崩溃，不推荐使用；
- 综合各方面考虑，推荐使用 revert-236-master 分支版本。

----

## rest-description 分支编译安装
- 将 pistache-rest-description.zip 放至 /usr/local/src 目录下并解压
  ```shell
    unzip pistache-rest-description.zip
  ```
- 创建编译目录，并编译安装
  ```shell
    cd pistache-rest-description
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local/pistache ..
    make
    make install
  ```
- 查看生成后的文件
    + 顺利的话，在 /usr/local/pistache 会生成三个文件夹 include, lib, share 。
    + 目录树如下:
      ```shell
        ├── include
        │   ├── rapidjson
        │   │   ├── error
        │   │   ├── internal
        │   │   └── msinttypes
        │   └── rest
        ├── lib
        │   ├── cmake
        │   │   └── RapidJSON
        │   └── pkgconfig
        └── share
            └── doc
                └── RapidJSON
                    └── examples
                        ├── capitalize
                        ├── condense
                        ├── jsonx
                        ├── messagereader
                        ├── parsebyparts
                        ├── pretty
                        ├── prettyauto
                        ├── schemavalidator
                        ├── serialize
                        ├── simpledom
                        ├── simplereader
                        ├── simplewriter
                        └── tutorial
      ```

## rest-description 分支测试
- 测试硬件及主机环境
    + Thinkpad T460(4 core + 8G)
    + 虚拟机版本 VMware 14.1.1 build-7528167
    + Red Hat Enterprise Linux Server 7.5 (Maipo)
    + Kernel 3.10.0-862.9.1.el7.x86_64 on an x86_64
    + gcc 版本 4.8.5
    + cmake 版本 2.8.12.2
- 测试程序在**rest-description测试目录下，分别是 restful_server.cc 和 restful_client.cc
  。编译如下:
  ```shell
    g++ -g -O0 -std=c++11 restful_server.cc -I/usr/local/pistache/include/rest  \
        /usr/local/pistache/lib/libnet.a -lpthread -o restful_server

    g++ -g -O0 -std=c++11 restful_client.cc -I/usr/local/pistache/include/rest  \
        /usr/local/pistache/lib/libnet.a -lpthread -o restful_client
  ```
- 运行测试程序
    + 在一个 shell 会话中打开服务端(绑定到 8090 端口)
      ```shell
        ./restful_server
      ```
    + 在另一个 shell 会话中打开客户端与服务端建立通信
      ```shell
        ./restful_client 127.0.0.1:8090
      ```
- 测试结果
    + 场景一: 连接池数量 64，客户端请求 50000 次，服务端接收请求向客户端发送响应(1983长度
      字符串)。服务端在处理每个请求后会 usleep = 10 微秒。
    + 结果一: 服务端可以完全处理 50000 次请求，客户端接收所有的响应时间不超过 6 秒。
    + 场景二: 连接池数量 64，客户端请求 50000 次，服务端接收请求向客户端发送响应(1983长度
      字符串和3966长度字符串按9:1比例发送，共计发送 50000 次)。服务端在处理每个请求后会 
      usleep = 50 微秒。
    + 结果二: 服务端可以完全处理 50000 次请求，客户端接收到 49963 条响应，接收这些响应用
      时不到 10 秒。
    + 场景三: 连接池数量 2000(可以通过 ulimit -n 4096 增加可开启描述符上限)，客户端请求 
      2000 次，服务端接收请求向客户端发送响应(3966长度字符串)，服务端在处理每个请求后会 
      usleep = 10 微秒。
    + 结果三: 服务端可以完全处理 2000 次请求，客户端接收到 1700 多条响应，接收这些响应用
      时不到 2 秒。
    + 通过调整一些参数，基本可以保证对于客户端的发送的每条请求，服务端都可以处理。但服务
      端发送的响应有部分丢失，原因暂未知。
- rest-description 实现的 restful 客户端和服务端说明
    + restful-client 采用连接池机制。连接池初始化时按固定数量创建，即使在请求多于现有连接
      池数量时，连接池数量也不再动态递增；
    + restful-client 使用长连接机制，这个可以通过抓包或通过 netstat 命令观察 ESTABLISHED 和
      TIME_WAIT 状态数量来确定；
    + 因为 restful-client 使用连接池，所以服务端收到的请求是"乱序"的；
    + restful-server 使用异步操作处理请求；
      
----

## io-reactor 分支下载及文件调整
- 克隆
  ```shell
    git clone https://github.com/oktal/pistache.git
  ```
  克隆所依赖的第三方库(需要一点时间)
  ```shell
    cd pistache
    git submodule update --init
  ```
  切换到 io-reactor 分支
  ```shell
    git checkout io-reactor
  ```
- 在编译之前，需要对 cmake 文件进行两处调整，否则无法编译通过。调整如下:
    + 主目录下的 CMakeLists.txt 文件，原本为:
      ```shell
        cmake_minimum_required (VERSION 2.8.7)
        project (pistache)
        include(CheckCXXCompilerFlag)

        CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
        CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
        if(COMPILER_SUPPORTS_CXX11)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        elseif(COMPILER_SUPPORTS_CXX0X)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        else()
                message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
        endif()

        include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
        file(GLOB HEADER_FILES "include/*.h")
        install(FILES ${HEADER_FILES} DESTINATION include/pistache)

        add_subdirectory (src)
        include_directories (src)

        add_subdirectory (examples)

        find_package(GTest)
        if (GTEST_FOUND)
            include_directories(${GTEST_INCLUDE_DIRS})
        else()
            ADD_SUBDIRECTORY (googletest-release-1.7.0)
            include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
        endif()

        enable_testing()
        add_subdirectory(tests)
      ```
      修改后为:
      ```shell
        cmake_minimum_required (VERSION 2.8.7)
        project (pistache)
        include(CheckCXXCompilerFlag)

        CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
        CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
        if(COMPILER_SUPPORTS_CXX11)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        elseif(COMPILER_SUPPORTS_CXX0X)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        else()
                message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
        endif()

        include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
        file(GLOB HEADER_FILES "include/*.h")
        install(FILES ${HEADER_FILES} DESTINATION include/pistache)

        add_subdirectory (third-party)
        include_directories(${RapidJSON_SOURCE_DIR}/include)

        add_subdirectory (src)
        include_directories (src)

        add_subdirectory (examples)

        ADD_SUBDIRECTORY (googletest-release-1.7.0)
        enable_testing()
        include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
        add_subdirectory(tests)
      ```
    + 将 master 分支下的 third-party 目录，拷贝到 io-reactor 分支主目录下，并向 third-party 目
      录添加一个 CMakeLists.txt 文件，内容如下:
      ```shell
        add_subdirectory(rapidjson)
      ```
    + 将调整后的文件打包为 pistache-io-reactor.zip

## io-reactor 分支编译安装
    + 将 pistache-io-reactor.zip 放至 /usr/local/src 目录下并解压
  ```shell
    unzip pistache-io-reactor.zip
  ```
- 创建编译目录，并编译安装
  ```shell
    cd pistache-io-reactor
    mkdir build
    cd build
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/pistache ..
    make
    make install
  ```
- 查看生成后的文件
    + 顺利的话，在 /usr/local/pistache 会生成三个文件夹 include, lib, share 。
    + 目录树如下:
      ```shell
        ├── include
        │   ├── pistache
        │   └── rapidjson
        │       ├── error
        │       ├── internal
        │       └── msinttypes
        ├── lib
        │   ├── cmake
        │   │   └── RapidJSON
        │   └── pkgconfig
        └── share
            └── doc
                └── RapidJSON
                    └── examples
                        ├── capitalize
                        ├── condense
                        ├── filterkey
                        ├── filterkeydom
                        ├── jsonx
                        ├── messagereader
                        ├── parsebyparts
                        ├── pretty
                        ├── prettyauto
                        ├── schemavalidator
                        ├── serialize
                        ├── simpledom
                        ├── simplereader
                        ├── simplewriter
                        └── tutorial
      ```

## 测试
   略