
### 参考

- [ZMQ中文指南](https://github.com/anjuke/zguide-cn)

### 版本

- [libzmq:4.3.2](https://github.com/zeromq/libzmq)
- [cppzmq:4.3.1](https://github.com/zeromq/cppzmq)

注意: zmq.hpp 4.3.1 是一个补丁版本

### 一般编译

```sh
  ./autogen.sh
  ./configure --prefix=/usr/local/zmq
  make
  make install
```

### 交叉编译

以 aarch64 为例:

```sh
  ./autogen.sh
  ./configure --host=aarch64-linux CC=/path/to/aarch64-none-linux-gnu-gcc AR=/path/to/aarch64-none-linux-gnu-ar CXX=/path/to/aarch64-none-linux-gnu-g++ LD=/path/to/aarch64-none-linux-gnu-ld --prefix=/tmp/aarch64
  make
  make install
```

### 内容

- [环境上下文](环境上下文.md)
- [消息收发](消息收发.md)
- [套接字选项](套接字选项.md)
- [通信模式](通信模式.md)

