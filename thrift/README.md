
# 使用 thrift C
- 参考
    + [花了一个星期，我终于把RPC框架整明白了！](https://developer.51cto.com/art/201906/597963.htm)
    + [c_glib Tutorial](http://thrift.apache.org/tutorial/c_glib)
    + [使用 GObject 库模拟类的数据封装形式](http://garfileo.is-programmer.com/2011/2/27/the-analog-of-classed-type-based-gobject.24798.html)
    + [thrift一个例子](https://blog.csdn.net/hbuxiaoshe/article/details/6558391/)
    
## 开发安装环境及配置
- 硬件及主机环境一
    + 虚拟机 VMware
    + Red Hat Enterprise Linux Server 7.5 (Maipo)
    + Kernel 3.10.0-862.9.1.el7.x86_64 on an x86_64
- 硬件及主机环境二
    + 虚拟机 VirtualBox
    + Debian 9.8.0
    + 4.9.0-8-amd64
    
## thrift 版本选择
- 采用 thrift-0.10.0 版本，版本地址为
    [thrift-0.10.0](http://archive.apache.org/dist/thrift/0.10.0/)
- 为什么要选择此版本?
  超过此版本的 thrift 在 configure 完成后，执行 make 时报如下错误:
  ```shell
    Making all in compiler/cpp
    make[2]: Entering directory `/usr/local/src/thrift-0.13.0/compiler/cpp'
    Making all in src
    make[3]: Entering directory `/usr/local/src/thrift-0.13.0/compiler/cpp/src'
    /bin/sh ../../../ylwrap thrift/thrifty.yy y.tab.c thrift/thrifty.cc y.tab.h `echo thrift/thrifty.cc | sed -e s/cc$/hh/ -e s/cpp$/hpp/ -e s/cxx$/hxx/ -e s/c++$/h++/ -e s/c$/h/` y.output thrift/thrifty.output -- byacc -d 
    byacc: e - line 1 of "/usr/local/src/thrift-0.13.0/compiler/cpp/src/thrift/thrifty.yy", syntax error
    %code requires {
    ^
  ```
  猜测可能是 gcc 编译器版本太低有关(当前gcc版本为 4.8.5)。thrift-0.10.0 是当前机器 gcc 编译器支持的最高版本。
- thrift 通过 IDL(接口定义语言)可以生成支持多种语言类型的源文件。本次 configure 后支持的语言类型文件如下:
  ```shell
    thrift 0.10.0

    Building Plugin Support ...... : no
    Building C++ Library ......... : no
    Building C (GLib) Library .... : yes
    Building Java Library ........ : no
    Building C# Library .......... : no
    Building Python Library ...... : yes
    Building Ruby Library ........ : no
    Building Haxe Library ........ : no
    Building Haskell Library ..... : no
    Building Perl Library ........ : no
    Building PHP Library ......... : yes
    Building Dart Library ........ : no
    Building Erlang Library ...... : no
    Building Go Library .......... : yes
    Building D Library ........... : no
    Building NodeJS Library ...... : no
    Building Lua Library ......... : no  
  ```
  因为 C++ 需要 boost 库的支持，所以这里是 no。
  本次编译时只要使其支持 c 语言类型即可。
- thrift 需要的 glib 库是 2.0 版本的，编译安装之前需要进行升级:
  ```shell
    yum install glibc-devel
  ```

## thrift-0.10.0 编译安装
- 安装依赖 (Debian)
  ```shell
    ## search for yacc
    apt-cache search yacc
    apt-get install byacc
    
    ## search for flex and install
    apt-get install flex
  ```
- 将 thrift-0.10.0.tar.gz 放到 /usr/local/src 目录下并解压:
  ```shell
    $ pwd
    /usr/local/src
    $ tar -zxvf thrift-0.10.0.tar.gz
    $ cd thrift-0.10.0
  ```
- 如果是从 github 上拉下来的，需要执行下列程序生成一个 configure 脚本
  ```shell
    $ ./bootstrap.sh
  ```
- 简单配置环境
  ```shell
    $ ./configure --prefix=/usr/local/thrift
  ```
  如果需要指定 boost 库，可以像下面这样:
  ```shell
    $ ./configure --with-boost=/path/to/boost/installed
  ```
  缺省情况下 thrift 以调试模式编译，可以像下面这样进行定制化编译:
  ```shell
    $ ./configure CXXFLAGS='-g -O2'
    $ ./configure CFLAGS='-g -O2'
    $ ./configure CPPFLAGS='-DDEBUG_MY_FEATURE'
  ```
  指定 gcc 编译器参数:
  ```shell
    $ ./configure --enable-coverage             # 启用 -fprofile-arcs -ftest-coverage 选项
  ```
- 编译 && 安装
  ```shell
    $ make
    $ make install
  ```
- 最后编译好的库会安装到 /usr/local/thrift 下，具体生成三个目录，如下:
  ```shell
    # pwd
    /usr/local/thrift
    # ls
    bin  include  lib
  ```
  目录树如下:
  ```shell
    ├── bin
    │   └── thrift
    ├── include
    │   └── thrift
    │       └── c_glib
    │           ├── config.h
    │           ├── processor
    │           │   ├── thrift_dispatch_processor.h
    │           │   └── thrift_processor.h
    │           ├── protocol
    │           │   ├── thrift_binary_protocol_factory.h
    │           │   ├── thrift_binary_protocol.h
    │           │   ├── thrift_compact_protocol_factory.h
    │           │   ├── thrift_compact_protocol.h
    │           │   ├── thrift_protocol_factory.h
    │           │   └── thrift_protocol.h
    │           ├── server
    │           │   ├── thrift_server.h
    │           │   └── thrift_simple_server.h
    │           ├── thrift_application_exception.h
    │           ├── thrift.h
    │           ├── thrift_struct.h
    │           └── transport
    │               ├── thrift_buffered_transport_factory.h
    │               ├── thrift_buffered_transport.h
    │               ├── thrift_fd_transport.h
    │               ├── thrift_framed_transport_factory.h
    │               ├── thrift_framed_transport.h
    │               ├── thrift_memory_buffer.h
    │               ├── thrift_server_socket.h
    │               ├── thrift_server_transport.h
    │               ├── thrift_socket.h
    │               ├── thrift_transport_factory.h
    │               └── thrift_transport.h
    └── lib
        ├── libthrift_c_glib.a
        ├── libthrift_c_glib.la
        ├── libthrift_c_glib.so -> libthrift_c_glib.so.0.0.0
        ├── libthrift_c_glib.so.0 -> libthrift_c_glib.so.0.0.0
        ├── libthrift_c_glib.so.0.0.0
        └── pkgconfig
            └── thrift_c_glib.pc
  ```

## thrift 的 c 语言使用示例
- 说明
    + 以官方给定的素材作为示例，只说操作，不说原因；
    + 操作目录为 ~test
- 环境配置步骤
    + 解压 thrift-0.10.0 后，其目录下有一个 tutorial 子目录，打开该子目录，有两个 IDL 文件:
        + shared.thrift
        + tutorial.thrift
      其中 tutorial.thrift 文件中已经通过下列语句包含有 shared.thrift 文件:
      ```shell
        include "shared.thrift"
      ```
    + 将上述两个 .thrift 文件拷贝出来，放到 ~test 目录下，执行如下命令:
      ```shell
        thrift -r --gen c_glib tutorial.thrift
      ```
      --gen 后指定生成的语言，这里是 c 语言。指定为 c 语言时，在生成程序时，只能使用 gcc ，无
      法使用 g++。
      顺利的话，在当前目录下会产生一个 gen-c_glib 文件夹，里面包含 c 类型的种类数据类型，树图
      如下:
      ```shell
        gen-c_glib/
        ├── calculator.c
        ├── calculator.h
        ├── shared_service.c
        ├── shared_service.h
        ├── shared_types.c
        ├── shared_types.h
        ├── tutorial_types.c
        └── tutorial_types.h
      ```
    + 因为是 c 语言，所以进入编译包 thrift-0.10.0/tutorial/c_glib 目录下，有下面两个文件:
      ```shell
        c_glib_client.c
        c_glib_server.c
      ```
      将上述两个文件拷贝至 ~test 目录下，此时 ~test 目录树如下:
      ```shell
        .
        ├── c_glib_client.c
        ├── c_glib_server.c
        ├── gen-c_glib
        │   ├── calculator.c
        │   ├── calculator.h
        │   ├── shared_service.c
        │   ├── shared_service.h
        │   ├── shared_types.c
        │   ├── shared_types.h
        │   ├── tutorial_types.c
        │   └── tutorial_types.h
        ├── shared.thrift
        └── tutorial.thrift
      ```
- 编译生成可执行文件
    + 在 ~test 目录下执行 gcc 生成。
    + 一共需要生成两个可执行文件，一个用作服务器，一个用作客户端，如下:
      ```shell
        gcc `pkg-config --cflags --libs gobject-2.0` ./gen-c_glib/*.c -I/usr/local/thrift/include \
            -L/usr/local/thrift/lib -lthrift_c_glib c_glib_server.c -o server
      
        gcc `pkg-config --cflags --libs gobject-2.0` ./gen-c_glib/*.c -I/usr/local/thrift/include \
            -L/usr/local/thrift/lib -lthrift_c_glib c_glib_client.c -o client
      ```
      当前环境下，执行 pkg-config --cflags --libs gobject-2.0 命令的结果是:
      ```shell
        -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include  -lgobject-2.0 -lglib-2.0
      ```
- 配置动态库路径，测试
    + 因为要用到 thrift c 动态库，通过下列命令进行简单配置:
      ```shell
        export LD_LIBRARY_PATH=/usr/local/thrift/lib:$LD_LIBRARY_PATH
      ```
    + 在一个 shell 会话中执行:
      ```shell
        ./server
      ```
      在另一个 shell 会话中执行:
      ```shell
        ./client
      ```
    + 顺利的话，两个 shell 会话分别会作如下打印:
      服务端 shell
      ```shell
        Starting the server...
        ping()
        add(1,1)
        calculate(1,{4,1,0})
        calculate(1,{2,15,10})
        getStruct(1)
    
      ```
      客户端 shell
      ```shell
        ping()
        1+1=2
        InvalidOperation: Cannot divide by 0
        15-10=5
        Check log: 5  
      ```